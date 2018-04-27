/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_room.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 10:48:01 by satkins           #+#    #+#             */
/*   Updated: 2018/04/27 10:11:10 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static size_t	to_end(t_meta *head, void *page_start, int p_size, size_t size)
{
	void	*ptr;
	size_t	dist;
	size_t	len;

	len = get_page_size(p_size);
	if (p_size == LARGE_IND)
	{
		if (((t_meta *)page_start)->len)
			size = ((t_meta *)page_start)->len;
		len = LARGE_PAGE;
	}
	ptr = page_start + len - sizeof(t_meta);
	dist = (size_t)(ptr - ((void *)head + sizeof(t_meta)));
	return (dist);
}

static void		clear_headers(t_meta *start_head)
{
	t_meta	*iter_head;
	size_t	len;

	iter_head = start_head;
	while (iter_head->len)
	{
		len = iter_head->len;
		if (iter_head != start_head)
			ft_bzero(iter_head, len + sizeof(t_meta));
		iter_head = (void *)iter_head + len + sizeof(t_meta);
	}
	start_head->len = 0;
}

size_t			check_contiguous(t_meta *start_head, size_t size, int p_size)
{
	t_meta	*iter_head;
	size_t	total_space;
	void	*next_page;

	iter_head = start_head;
	next_page = NULL;
	total_space = 0;
	while (!iter_head->used)
	{
		if (iter_head->next_page)
			next_page = iter_head->next_page;
		if (!iter_head->len)
		{
			clear_headers(start_head);
			start_head->next_page = next_page;
			if (to_end(start_head, start_head->start, p_size, size) < size)
				return (0);
			return (size);
		}
		total_space += iter_head->len;
		iter_head = iter_head->mem_seg + iter_head->len;
	}
	if (total_space >= size)
		return (total_space);
	return (0);
}
