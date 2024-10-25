/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:44:08 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/25 17:46:09 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

static void	print_part(const byte *raw, size_t size, const char *label, const size_t begin, bool quick);
static void	print_line(const byte *raw, const long size, const char *label, const long label_size);
static str	elf_header_label(Elf64_Ehdr header);

#define LABEL_PER_LINE 42

void	print_elf(FILE * const f) {
	Elf64_Ehdr	elf_header;
	Elf64_Phdr	prog_header;
	Elf64_Shdr	section_header;
	size_t		current_offset = 0;
	str			elf_label;

	fread(&elf_header, sizeof(Elf64_Ehdr), 1, f); 

	printf("\033[32m");

	elf_label = elf_header_label(elf_header);
	print_part((const byte *)&elf_header, sizeof(Elf64_Ehdr), elf_label, current_offset, false);
	current_offset += sizeof(Elf64_Ehdr);

	fseek(f, elf_header.e_phoff, SEEK_SET);
	current_offset = elf_header.e_phoff;
	fread(&prog_header, sizeof(Elf64_Phdr), 1, f); 

	printf("\033[33m");
	print_part((const byte *)&prog_header, sizeof(Elf64_Phdr), "Program Header Table", current_offset, false);
	current_offset += sizeof(Elf64_Phdr);

	fseek(f, elf_header.e_shoff, SEEK_SET);
	current_offset = elf_header.e_shoff;
	fread(&section_header, sizeof(Elf64_Shdr), 1, f); 

	printf("\033[34m");
	print_part((const byte *)&section_header, sizeof(Elf64_Shdr), "Section Header Table", current_offset, false);
	current_offset += sizeof(Elf64_Shdr);
}


static void	print_part(const byte *raw, size_t size, const char *label, const size_t begin, bool quick) {

	size_t	lines = size / 16 + (size % 16 ? 1 : 0);
	size_t	label_size = strlen(label);

	(void)quick;

	printf("+--------------------------------------------------+ 0x%08x\n", (unsigned int)begin);
	for (size_t i = 0; i < lines; i++) {
		print_line(raw + (i * 16), size - (i * 16),
			 label ? label + (i * LABEL_PER_LINE) : NULL, label_size - (i * LABEL_PER_LINE));
	}
	printf("+--------------------------------------------------+ 0x%08x\n", (unsigned int)(begin + size));

}

static void	print_line(const byte *raw, const long size, const char *label, const long label_size) {

	printf("| ");
	for (int i = 0; i < 16; i++) {
		if (i == 8)
			printf(" ");
		if (i > size) {
			printf("   ");
		} else {
			printf("%02x ", raw[i]);
		}
	}
	printf("| ");
	if (label) {
		for (int i = 0; i < LABEL_PER_LINE; i++) {
			if (i <= label_size) {
				printf("%c", label[i]);
			}
		}
	}
	printf("\n");
}

static str	elf_header_label(Elf64_Ehdr header) {
	str	s;

	s = malloc(42 * 4);
	if (s == NULL)
		return s;

	sprintf(s, "Entry point: 0x%08x", (unsigned int)header.e_entry);

	return s;
}

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
