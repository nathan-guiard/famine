/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:42 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/30 12:52:57 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
#define FAMINE_H

#include <stdint.h>

typedef	char *			str;
typedef	unsigned char	byte;

#include "asm_macros.h"

// Debug purpose
#ifdef DEBUG

# include <stdio.h>

# define printf(...) printf(__VA_ARGS__)
# define perror(x) perror(x)
# define fflush(x) fflush(x)

# define FILE_LINE(x) __FILE__ ":%d " x, __LINE__

#else

# define printf(x)
# define perror(x)
# define fflush(x)

#endif

#endif
