/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profiling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:58:52 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/05 11:36:17 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

profiling	get_profiling(byte *start_rip) {
	profiling	ret;
	size_t		i = 0;
	ret.start_rip = start_rip;

	for (; ((uint128_t *)start_rip)[i]; i++)
		i++;

	ret.size = i * sizeof(uint128_t);

	printf("0x%lx | 0x%lx\n", ret.size, (((uint64_t)ret.start_rip) & 0xfff) + 0x1000); 

	size_t	stringok = 0x000a31, stringko = 0x000a30;

	if (ft_memcmp(start_rip + ret.size + 0x10, (byte *)SIGNATURE, SIGNATURE_LEN)) {
		write(stringok, 1, (byte *)&stringok, 2);	
	} else {
		write(stringko, 1, (byte *)&stringko, 2);
	}

	return ret;
}
