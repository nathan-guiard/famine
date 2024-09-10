/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/09/10 03:22:32 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void display_elf_header(Elf64_Ehdr elf_header) {
	printf("ELF Header:\n");
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++) {
        printf("%02x ", elf_header.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             ");
    switch (elf_header.e_ident[EI_CLASS]) {
        case ELFCLASS32:
            printf("ELF32\n");
            break;
        case ELFCLASS64:
            printf("ELF64\n");
            break;
        default:
            printf("Invalid class\n");
            break;
    }

    printf("  Data:                              ");
    switch (elf_header.e_ident[EI_DATA]) {
        case ELFDATA2LSB:
            printf("2's complement, little endian\n");
            break;
        case ELFDATA2MSB:
            printf("2's complement, big endian\n");
            break;
        default:
            printf("Invalid data encoding\n");
            break;
    }

    printf("  Version:                           %d\n", elf_header.e_ident[EI_VERSION]);
    printf("  OS/ABI:                            %d\n", elf_header.e_ident[EI_OSABI]);
    printf("  ABI Version:                       %d\n", elf_header.e_ident[EI_ABIVERSION]);
    printf("  Type:                              0x%x\n", elf_header.e_type);
    printf("  Machine:                           0x%x\n", elf_header.e_machine);
    printf("  Version:                           0x%x\n", elf_header.e_version);
    printf("  Entry point address:               0x%lx\n", elf_header.e_entry);
    printf("  Start of program headers:          %ld (bytes into file)\n", elf_header.e_phoff);
    printf("  Start of section headers:          %ld (bytes into file)\n", elf_header.e_shoff);
    printf("  Flags:                             0x%x\n", elf_header.e_flags);
    printf("  Size of this header:               %d (bytes)\n", elf_header.e_ehsize);
    printf("  Size of program headers:           %d (bytes)\n", elf_header.e_phentsize);
    printf("  Number of program headers:         %d\n", elf_header.e_phnum);
    printf("  Size of section headers:           %d (bytes)\n", elf_header.e_shentsize);
    printf("  Number of section headers:         %d\n", elf_header.e_shnum);
    printf("  Section header string table index: %d\n", elf_header.e_shstrndx);
}

int main(int argc, char **argv) {
	FILE	*f;
	Elf64_Ehdr	elf_header;

	(void)argc;

	if (valid_file(&f, argv[1])) {
		printf("Invalid\n");
	} else {
		printf("valid\n");
	}

	fseek(f, 0, 0);

	fread(&elf_header, sizeof(Elf64_Ehdr), 1, f);

	display_elf_header(elf_header);
}