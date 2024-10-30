/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/30 16:36:44 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void _start() {
	int64_t		ret;
	uint64_t	fd;
	struct stat file = {0};
	str			file_name = "exemple/hello_world";
	str			file_origin = NULL;

	open(fd, file_name, O_RDWR);

	printf("fd: %d\n", (int)fd);

	fstat(ret, fd, &file);
	if (ret)
		printf(FILE_LINE("fstat failed: %ld\n"), ret);

	printf("Size of the file: 0x%lx\n", file.st_size);

	mmap(file_origin, NULL, file.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (file_origin == MAP_FAILED)
		printf(FILE_LINE("fstat failed: %p\n"), file_origin);

	exit(0);
}

