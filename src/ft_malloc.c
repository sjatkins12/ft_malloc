/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 19:49:45 by satkins           #+#    #+#             */
/*   Updated: 2018/04/27 10:20:43 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

pthread_rwlock_t	g_rwlock[3] =
{
	PTHREAD_RWLOCK_INITIALIZER,
	PTHREAD_RWLOCK_INITIALIZER,
	PTHREAD_RWLOCK_INITIALIZER
};

size_t				get_page_size(int p_size)
{
	static int		page = 0;
	size_t			len;

	if (!page)
		page = getpagesize();
	if (p_size == TINY_IND)
		len = (TINY + sizeof(t_meta)) * 100 + sizeof(t_meta);
	else if (p_size == SMALL_IND)
		len = (SMALL + sizeof(t_meta)) * 100 + sizeof(t_meta);
	else
		len = 0;
	return (len + page - (len % page));
}

t_meta				*init_page(size_t p_size, size_t size)
{
	size_t			len;
	void			*page;

	len = get_page_size(p_size);
	if (p_size == LARGE_IND)
		len = LARGE_PAGE;
	page = NULL;
	if ((page = mmap(page, len, PROT_ALL, ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	page_creation_debug(page, len);
	if (g_pages[MALLOC_CHECK_IND] == NULL)
		g_pages[MALLOC_CHECK_IND] = page;
	g_pages[MALLOC_CHECK_MAX] = page + len - 1;
	return (page);
}

static void			*find_space(size_t p_size, size_t size)
{
	t_meta			*head;

	if (g_pages[p_size] == NULL)
	{
		if ((head = init_page(p_size, size)) == NULL)
			return (NULL);
		head->start = head;
		set_meta(head, size, p_size);
		g_pages[p_size] = (void *)head;
	}
	else if ((head = iter_headers(g_pages[p_size], size, p_size)) == NULL)
		return (NULL);
	return (head->mem_seg);
}

int					check_limit(size_t size)
{
	struct rlimit	lim;

	if (getrlimit(RLIMIT_DATA, &lim) == -1)
		return (0);
	if (size >= lim.rlim_cur)
		return (0);
	return (1);
}

void				*malloc(size_t size)
{
	int				p_size;
	void			*ptr;

	if (size <= TINY)
		p_size = TINY_IND;
	else if (size <= SMALL)
		p_size = SMALL_IND;
	else if (check_limit(size) == 0)
		return (NULL);
	else
		p_size = LARGE_IND;
	if (!size)
		size = 1;
	pthread_rwlock_wrlock(&(g_rwlock[p_size]));
	ptr = find_space(p_size, size);
	pthread_rwlock_unlock(&(g_rwlock[p_size]));
	return (ptr);
}
