/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/28 17:16:47 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

int main(int argc, char **argv) {
	FILE		*f;
	Elf64_Ehdr	elf_h;
	Elf64_Phdr	*progs_h;

	if (argc != 2)
		return 1;

	if (valid_file(&f, argv[1])) {
		printf("Invalid\n");
		return 1;
	} else {
		printf("valid\n");
	}

	fseek(f, 0, SEEK_SET);
	
	fread(&elf_h, sizeof(Elf64_Ehdr), 1, f);

	Elf64_Phdr new_prog_h = {
		.p_type		= PT_LOAD,
		.p_flags	= PF_R + PF_X,
		.p_offset	= 0x42,
		.p_vaddr	= 0x420,
		.p_paddr	= 0x4200,
		.p_filesz	= 0x80,
		.p_memsz	= 0x80,
		.p_align	= 0x8,
	};

	progs_h = get_program_headers(f, elf_h);
	if (progs_h == NULL) {
		printf("Error get_program_headers\n");
		return 1;
	}

	// 13 -> test purposes
	size_t	new_program_header_offset = elf_h.e_phoff + (elf_h.e_phentsize * (13));

	if (insert_program_header(f, progs_h, elf_h.e_phnum,
						   new_prog_h, new_program_header_offset) == 0)
	{
		printf(FILE_LINE("Error insert_program_header\n"));
		return 1;
	}
	
	if (change_phnum(f, 14)) {
		printf("Error changing phnum\n");
		free(progs_h);
		return 1;
	}

	fflush(f);

	print_elf(f);
}

