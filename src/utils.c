/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:23:11 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/31 13:30:01 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

size_t	ft_strlen(str buff) {
	size_t	index = 0;

	while (buff[index])
		index++;

	return index;
}

//	Returns false if the two chuncks are not the same
bool	ft_memcmp(const byte *a, const byte *b, size_t size) {
	size_t	i = 0;

	while (i < size) {
		if (a[i] != b[i])
			return false;
		i++;
	}

	return true;
}
