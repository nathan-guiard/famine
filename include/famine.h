/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:42 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/25 16:30:04 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
#define FAMINE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <stdbool.h>
#include <string.h>

typedef	char *			str;
typedef	unsigned char	byte;

//	print_elf
void	display_elf_header(Elf64_Ehdr elf_header);
void	print_elf(FILE * const f);

//	valid_file
bool	valid_file(FILE **f, str filename);

//	file_operations.c
bool	insert(FILE *f, const void *data, const size_t size, size_t offset);


#ifdef DEBUG
# define printf(...) printf(__VA_ARGS__)
# define perror(x) perror(x)
# define fflush(x) fflush(x)
#else
# define printf(x)
# define perror(x)
# define fflush(x)
#endif

#endif
