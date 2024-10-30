/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/30 18:52:17 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void _start() {
	int64_t		ret = 0;
	uint64_t	fd;
	uint64_t	dir_fd;
	struct stat file_stat = {0};
	str			file_name = "exemple/hello_world";
	byte		*file_origin = NULL;
	byte		buff[4096];
	dirent		*d;

	open(dir_fd, "/bin", O_RDONLY);	

	getdents(ret, dir_fd, buff, 4096);
	while (ret > 0) {
		int64_t	d_offset = 0;
		while (d_offset < ret) {
			d = (dirent *)((uint64_t)(buff) + d_offset);
			printf("%s\n", d->d_name);
			d_offset += d->d_reclen;
		}
		getdents(ret, dir_fd, buff, 4096);
	}
	
	exit(0);
	// ---
	open(fd, file_name, O_RDWR);

	printf("fd: %d\n", (int)fd);

	fstat(ret, fd, &file_stat);
	if (ret)
		printf(FILE_LINE("fstat failed: %ld\n"), ret);

	printf("Size of the file: 0x%lx\n", file_stat.st_size);

	mmap(file_origin, NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (file_origin == MAP_FAILED)
		printf(FILE_LINE("fstat failed: %p\n"), file_origin);

	munmap(ret, file_origin, file_stat.st_size);
	close(fd);
	
	exit(0);
}

size_t	ft_strlen(str buff) {
	size_t	index = 0;

	while (buff[index])
		index++;

	return index;
}

