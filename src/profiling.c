/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profiling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:58:52 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/05 12:16:57 by nguiard          ###   ########.fr       */
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

	return ret;
}
