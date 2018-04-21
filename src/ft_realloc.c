/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 14:13:58 by satkins           #+#    #+#             */
/*   Updated: 2018/03/17 15:58:12 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static int	is_space(t_meta *met_head, size_t size)
{
	t_meta	*met_iter;
	size_t	curr_len;

	curr_len = met_head->len;
	met_iter = (void *)met_head + sizeof(t_meta) + met_head->len;
	while ((void *)met_iter < met_head->start + get_page_size(met_head->p_index) - sizeof(t_meta))
	{
		if (!met_iter->len)
		{
			if (size + met_head->mem_seg < met_head->start + get_page_size(met_head->p_index) - sizeof(t_meta))
			{
				met_head->next_page = met_iter->next_page;
				met_head->len = size;
				return (1);
			}
			return (0);
		}
		if (met_iter->used)
			return (0);
		curr_len += met_iter->len + sizeof(t_meta);
		if (size <= curr_len)
		{
			met_head->next_page = met_iter->next_page;
			met_head->len = curr_len;
			return (1);
		}
		met_iter = (void *)met_iter + met_iter->len + sizeof(t_meta);
	}
	return (0);
}

void		*realloc(void *ptr, size_t size)
{
	t_meta	*met_head;
	void	*tmp;

	if (!ptr)
		return (malloc(size));
	met_head = ptr - sizeof(t_meta);
	if (!validate_csum(met_head))
		return (NULL);
	if (size <= met_head->len)
		return (ptr);
	else if (met_head->p_index != LARGE_IND && is_space(met_head, size))
		return (ptr);
	if (!(tmp = malloc(size)))
		return (NULL);
	ft_memcpy(tmp, ptr, met_head->len);
	free(ptr);
	return (tmp);	
}
