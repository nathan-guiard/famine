/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/25 16:32:13 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

int main(int argc, char **argv) {
	FILE		*f;
//	Elf64_Ehdr	elf_header;

	if (argc != 2)
		return 1;

	if (valid_file(&f, argv[1])) {
		printf("Invalid\n");
		return 1;
	} else {
		printf("valid\n");
	}

	fseek(f, 0, 0);

	print_elf(f);
}
