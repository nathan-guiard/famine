/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:42 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/30 18:34:46 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
#define FAMINE_H

#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef	char *			str;
typedef	unsigned char	byte;

typedef struct dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_reclen;
	char			d_name[256];
}	dirent;

#include "asm_macros.h"

//	Main
size_t	ft_strlen(str buff);

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
