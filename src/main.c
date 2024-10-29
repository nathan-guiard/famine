/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/29 12:13:25 by nguiard          ###   ########.fr       */
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

	progs_h = get_program_headers(f, elf_h);
	if (progs_h == NULL) {
		printf("Error get_program_headers\n");
		return 1;
	}

	print_elf(f);
}

