/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 13:56:55 by satkins           #+#    #+#             */
/*   Updated: 2018/04/26 10:01:21 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	*g_pages[5] = {NULL, NULL, NULL, NULL, NULL};

int			validate_addr(void *ptr)
{
	void	*malloc_check;

	if (!ptr
		|| !(malloc_check = g_pages[MALLOC_CHECK_IND])
		|| malloc_check > ptr
		|| !g_pages[MALLOC_CHECK_MAX]
		|| ptr > g_pages[MALLOC_CHECK_MAX])
		return (0);
	return (1);
}

void		free(void *ptr)
{
	t_meta	*met_head;

	if (!validate_addr(ptr))
		return ;
	met_head = ptr - sizeof(t_meta);
	if (!validate_csum(met_head))
		return ;
	pthread_rwlock_wrlock(&(g_rwlock[met_head->p_index]));
	met_head->used = 0;
	ft_bzero(ptr, met_head->len);
	pthread_rwlock_unlock(&(g_rwlock[met_head->p_index]));	
}
