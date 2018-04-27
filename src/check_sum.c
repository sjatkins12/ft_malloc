/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sum.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/26 09:58:39 by satkins           #+#    #+#             */
/*   Updated: 2018/04/26 10:00:25 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

int		make_sum(t_meta *met_head)
{
	int		i;
	int		sum;

	met_head->chk_sum = 0;
	sum = 0;
	i = -1;
	while (++i < (int)sizeof(t_meta))
		sum += ((unsigned char *)met_head)[i];
	return (sum);
}

int		validate_csum(t_meta *met_head)
{
	int		i;
	uint8_t	val;
	uint8_t	sum;

	if (!met_head || !met_head->used)
		return (0);
	sum = 0;
	val = met_head->chk_sum;
	met_head->chk_sum = 0;
	i = -1;
	while (++i < (int)sizeof(t_meta))
		sum += ((unsigned char *)met_head)[i];
	met_head->chk_sum = val;
	return (sum == val);
}
