/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:47:01 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/31 12:07:44 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

bool	infect(const str path) {
	int64_t	fd;
	int64_t	ret;
	struct stat file_stat = {0};
	byte		*file_origin = NULL;

	(void)fd;
	(void)ret;

	open(fd, path, O_RDWR);

	printf("fd: %d\n", (int)fd);

	fstat(ret, fd, &file_stat);
	if (ret)
		printf(FILE_LINE("fstat failed: %ld\n"), ret);

	printf("Size of the file: %ld\n", file_stat.st_size);

	mmap(file_origin, NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (file_origin == MAP_FAILED)
		printf(FILE_LINE("fstat failed: %p\n"), file_origin);

	munmap(ret, file_origin, file_stat.st_size);
	close(fd);

	return false;
}
