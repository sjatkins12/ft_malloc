/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 13:56:55 by satkins           #+#    #+#             */
/*   Updated: 2018/03/17 15:57:46 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

int	validate_csum(t_meta *met_head)
{
	int		i;
	uint8_t	val;
	uint8_t	sum;

	sum = 0;
	val = met_head->chk_sum;
	i = -1;
	while (++i < (int)sizeof(t_meta))
		sum += ((unsigned char *)met_head)[i];
	return (sum == val);
}

void		free(void *ptr)
{
	t_meta	*met_head;

	if (!ptr)
		return ;
	met_head = ptr - sizeof(t_meta);
	if (!validate_csum(met_head))
		return ;
	met_head->used = 0;
	ft_bzero(ptr, met_head->len);
}
