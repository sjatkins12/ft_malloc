/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 14:13:58 by satkins           #+#    #+#             */
/*   Updated: 2018/04/26 10:05:47 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static int			is_space(t_meta *iter_head, size_t size)
{
	size_t	space;

	pthread_rwlock_wrlock(&(g_rwlock[iter_head->p_index]));
	if ((space = check_contiguous(iter_head, size, iter_head->p_index)))
	{
		if (iter_head->next_page)
			N_HEAD = iter_head->next_page;
		iter_head->next_page = NULL;
		set_meta(iter_head, space, iter_head->p_index);
		pthread_rwlock_unlock(&(g_rwlock[iter_head->p_index]));
		return (1);
	}
	pthread_rwlock_unlock(&(g_rwlock[iter_head->p_index]));
	return (0);
}

static size_t		seg_sizes(int p_size)
{
	if (p_size == SMALL_IND)
		return (SMALL);
	else if (p_size == TINY_IND)
		return (TINY);
	else if (p_size == LARGE_IND)
		return (0);
	return (0);
}

void				*realloc(void *ptr, size_t size)
{
	t_meta	*met_head;
	void	*tmp;

	if (!validate_addr(ptr))
		return (malloc(size));
	met_head = ptr - sizeof(t_meta);
	if (!validate_csum(met_head))
		return (NULL);
	if (size <= met_head->len)
		return (ptr);
	else if (size && seg_sizes(met_head->p_index) >= size &&
		is_space(met_head, size))
		return (ptr);
	if (!(tmp = malloc(size)))
		return (NULL);
	if (!ft_memcpy(tmp, ptr, met_head->len))
		return (NULL);
	free(ptr);
	return (tmp);
}
