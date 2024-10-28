/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program_header.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:20:05 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/28 17:14:13 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

#define	E_PHNUM_OFFSET EI_NIDENT + 2 + 2 + 4 + 8 + 8 + 8 + 4 + 2 + 2

// Insert a program header at the end of the current ones
//
// Returns 0 on error
size_t	insert_program_header(FILE *f, Elf64_Phdr *progs_h, size_t size_progs_h,
							 Elf64_Phdr new_prog_h, size_t offset)
{
	size_t	highest = 0;
	size_t	highest_vram = 0;

	for (size_t i = 0; i < size_progs_h; i++) {
		if (progs_h[i].p_offset + progs_h[i].p_filesz > highest) {
			highest = progs_h[i].p_offset + progs_h[i].p_filesz;  

			// Respect alignment (if ever)
			if (highest % progs_h[i].p_align != 0)
				highest += highest % progs_h[i].p_align;
		}

		if (progs_h[i].p_vaddr + progs_h[i].p_memsz > highest_vram) {
			highest_vram = progs_h[i].p_vaddr + progs_h[i].p_memsz; 
		}
	}

	new_prog_h.p_offset	= highest;
	new_prog_h.p_vaddr	= highest_vram;

	fseek(f, offset, SEEK_SET);
	if (ferror(f))
		return 0;

	fwrite(&new_prog_h, sizeof(Elf64_Phdr), 1, f);
	if (ferror(f))
		return 0;
	
	return highest;
}

//	Change the Elf64_Ehdr.e_phnum field on a ELF file
//
//	Returns true on error
bool	change_phnum(FILE *f, Elf64_Half new_amount) {
	fseek(f, E_PHNUM_OFFSET, SEEK_SET);
	if (ferror(f)) {
		printf(FILE_LINE("Couldn't fseek()\n"));
		return true;
	}

	fwrite(&new_amount, sizeof(Elf64_Half), 1, f);
	if (ferror(f)) {
		printf(FILE_LINE("Couldn't fwrite()\n"));
		return true;
	}
	
	return false;
}

//	Gets every program header and returns it
//
//	Return NULL on error
Elf64_Phdr	*get_program_headers(FILE *f, Elf64_Ehdr elf_h) {
	Elf64_Phdr	*result = NULL;

	fseek(f, elf_h.e_phoff, SEEK_SET);
	if (ferror(f)) {
		printf(FILE_LINE("Couldn't fseek()\n")); 
		return NULL;
	}
	
	result = calloc(sizeof(Elf64_Phdr), elf_h.e_phnum);
	if (result == NULL) {
		printf(FILE_LINE("Couldn't calloc()\n"));
		return NULL;
	}

	fread(result, sizeof(Elf64_Phdr), elf_h.e_phnum, f);
	if (ferror(f)) {
		printf(FILE_LINE("Couldn't fread()\n")); 
		free(result);
		return NULL;
	}

	return result;

}
