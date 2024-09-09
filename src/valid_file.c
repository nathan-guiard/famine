/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:40:41 by nguiard           #+#    #+#             */
/*   Updated: 2024/09/09 11:23:56 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

bool	valid_file(FILE **f, str filename) {
	Elf64_Ehdr	elf_header;

	*f = fopen(filename, "r");

	if (*f == NULL) {
		printf("Could not open %s: ", filename);
		fflush(*f);
		perror(filename);
		return true;
	}

	if (fread(&elf_header, sizeof(Elf64_Ehdr), 1, *f) != sizeof(Elf64_Ehdr)) {
		printf("File shorter than an ELF header\n");
		return true;
	}
	

	if (elf_header.e_ident[4] != ELFCLASS64) {
		printf("Not 64 bit\n");
		return true;
	}

	if (elf_header.e_ident[5] != ELFDATA2LSB) {
		printf("Not little endian\n");
		return true;
	}

	return false;
}