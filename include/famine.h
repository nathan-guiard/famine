/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:42 by nguiard           #+#    #+#             */
/*   Updated: 2024/09/09 11:20:12 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
#define FAMINE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <stdbool.h>

typedef	char * str;

bool	valid_file(FILE **f, str filename);

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