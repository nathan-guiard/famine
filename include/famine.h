/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:42 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/31 11:08:56 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
#define FAMINE_H

#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

typedef	char *			str;
typedef	unsigned char	byte;

typedef struct dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_reclen;
	char			d_name[256];
}	dirent;

#define DT_UNKNOWN  0   // Unknown file type
#define DT_FIFO     1   // Named pipe (FIFO)
#define DT_CHR      2   // Character device
#define DT_DIR      4   // Directory
#define DT_BLK      6   // Block device
#define DT_REG      8   // Regular file
#define DT_LNK     10   // Symbolic link
#define DT_SOCK    12   // Socket
#define DT_WHT     14   // Whiteout (used in some special file systems)

#include "asm_macros.h"

//	Main
size_t	ft_strlen(str buff);

//	Infection
bool	infect(const str path);
// Debug purpose
#ifdef DEBUG

# include <stdio.h>

# define printf(...) printf(__VA_ARGS__)
# define perror(x) perror(x)
# define fflush(x) fflush(x)

# define FILE_LINE(x) __FILE__ ":%d : " x, __LINE__

#else

# define printf(x)
# define perror(x)
# define fflush(x)

#endif

#endif
