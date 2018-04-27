/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/24 16:01:58 by satkins           #+#    #+#             */
/*   Updated: 2018/04/26 09:54:15 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	show_pages(t_meta *page_start)
{
	if (!page_start)
		return ;
	while (page_start->len)
	{
		if (page_start->used)
		{
			ft_printf("%p - ", page_start->mem_seg);
			ft_printf("%p", page_start->mem_seg + page_start->len);
			ft_printf(" : %zu bytes\n", page_start->len);
		}
		page_start = page_start->mem_seg + page_start->len;
	}
	show_pages(page_start->next_page);
}

void	show_large(void)
{
	ft_printf("LARGE : %p\n", g_pages[LARGE_IND]);
	show_pages(g_pages[LARGE_IND]);
}

void	show_small(void)
{
	ft_printf("SMALL : %p\n", g_pages[SMALL_IND]);
	show_pages(g_pages[SMALL_IND]);
}

void	show_tiny(void)
{
	ft_printf("TINY : %p\n", g_pages[TINY_IND]);
	show_pages(g_pages[TINY_IND]);
}

void	show_alloc_mem(void)
{
	pthread_rwlock_rdlock(&(g_rwlock[TINY_IND]));
	pthread_rwlock_rdlock(&(g_rwlock[SMALL_IND]));
	pthread_rwlock_rdlock(&(g_rwlock[LARGE_IND]));
	show_tiny();
	show_small();
	show_large();
	pthread_rwlock_unlock(&(g_rwlock[TINY_IND]));
	pthread_rwlock_unlock(&(g_rwlock[SMALL_IND]));
	pthread_rwlock_unlock(&(g_rwlock[LARGE_IND]));
}
