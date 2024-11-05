/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profiling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:58:52 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/05 17:45:47 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

profiling	get_profiling(byte *start_rip) {
	profiling	ret;
	size_t		i = 0;
	ret.start_rip = start_rip;

	for (; ((uint128_t *)start_rip)[i] + ((uint128_t *)start_rip)[i + 1]; i++)
		;

	i *= sizeof(uint128_t);

	for (; start_rip[i] == 0; i--)
		;
	
	ret.size = i + 1;

	if (*(start_rip + ret.size + SIGNATURE_OFFSET) == 0) {
		ret.original = true;
		ret.signature = (byte *)SIGNATURE;
	} else {
		ret.original = false;
		ret.signature = start_rip + ret.size + SIGNATURE_OFFSET;
	}

	return ret;
}
