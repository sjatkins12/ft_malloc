/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background_cleaner.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/26 09:56:54 by satkins           #+#    #+#             */
/*   Updated: 2018/04/26 09:58:08 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	check_munmap(t_meta *page_start, t_meta *prev_tail)
{
	t_meta	*iter;
	int		mun_flag;
	void	*next_page;
	size_t	len;

	if (!(iter = page_start))
		return ;
	mun_flag = 0;
	while (iter->len)
	{
		if (iter->used)
			mun_flag = 1;
		iter = iter->mem_seg + iter->len;
	}
	next_page = iter->next_page;
	if (!mun_flag)
	{
		if (prev_tail)
			prev_tail->next_page = next_page;
		else
			g_pages[page_start->p_index] = next_page;
		len = get_page_size(page_start->p_index);
		if (page_start->p_index == LARGE_IND)
			len = ((page_start->len + sizeof(t_meta) * 2) + 
				(len - (page_start->len + sizeof(t_meta) * 2) % len));
		munmap(page_start, len);
	}
	else
		prev_tail = iter;
	check_munmap(next_page, prev_tail);
}

void	*background_cleaner(__attribute__((unused))void *vargp)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < 3)
			if (pthread_rwlock_trywrlock(&(g_rwlock[i])) == EXIT_SUCCESS)
			{
				check_munmap(g_pages[i], NULL);
				pthread_rwlock_unlock(&(g_rwlock[i]));
			}
		sleep(GARBAGE_INTERVAL);
	}
}

__attribute__((constructor)) static void init_cleaner(void)
{
	pthread_t		thread;

	pthread_create(&thread, NULL, background_cleaner, NULL);
}
