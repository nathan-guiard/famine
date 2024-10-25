/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:55:09 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/25 14:55:12 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

// Returns true on error
// Returns false if everything went right
bool	insert(FILE *f, const void *data, const size_t size, size_t offset) {
	size_t	file_size = 0, read_size = 0;
	byte	*after_data = NULL;

	if (!f)
		return true;

	fseek(f, 0, SEEK_END);

	file_size = ftell(f);

	if (file_size < offset) {
		return true;
	}
	read_size = file_size - offset;
	fseek(f, offset, SEEK_SET);
	after_data = calloc(read_size, sizeof(byte));
	if (after_data == NULL)
		return true;

	fread(after_data, 1, read_size, f);
	if (ferror(f)) {
		free(after_data);
		return true;
	}

	fseek(f, offset, SEEK_SET);
	fwrite(data, size, 1, f);
	if (ferror(f)) {
		free(after_data);
		return true;
	}

	fwrite(after_data, read_size, 1, f);
	free(after_data);
	if (ferror(f))
		return true;
	return false;
}
