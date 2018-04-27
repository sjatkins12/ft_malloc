/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter_pages.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/21 16:50:47 by satkins           #+#    #+#             */
/*   Updated: 2018/04/27 10:23:24 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void			*set_meta(t_meta *meta, size_t size, int p_size)
{
	if (!meta)
		return (NULL);
	meta->mem_seg = (void *)meta + sizeof(t_meta);
	meta->len = size;
	meta->p_index = p_size;
	meta->used = 1;
	meta->chk_sum = make_sum(meta);
	return (meta);
}

static	void	set_head(t_meta *head, void *page_start)
{
	if (!head || !page_start)
		return ;
	head->start = page_start;
	head->mem_seg = (void *)head + sizeof(t_meta);
}

void			*iter_headers(void *head, size_t size, int p_size)
{
	t_meta	*iter_head;
	size_t	space;

	if (!(iter_head = head))
		return (NULL);
	while (1)
	{
		set_head(iter_head, head);
		if (!iter_head->used
			&& (space = check_contiguous(iter_head, size, p_size)))
		{
			if (iter_head->next_page)
				N_HEAD = iter_head->next_page;
			iter_head->next_page = NULL;
			return (set_meta(iter_head, space, p_size));
		}
		if (!iter_head->len)
		{
			if (!iter_head->next_page)
				iter_head->next_page = init_page(p_size, size);	
			return (iter_headers(iter_head->next_page, size, p_size));
		}
		iter_head = (void *)iter_head + sizeof(t_meta) + iter_head->len;
	}
}
