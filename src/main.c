/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:01:26 by nguiard           #+#    #+#             */
/*   Updated: 2024/09/09 11:19:02 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

int main(int argc, char **argv) {
	FILE	*f;

	(void)argc;

	if (valid_file(&f, argv[1])) {
		printf("Invalid\n");
	} else {
		printf("valid\n");
	}
}