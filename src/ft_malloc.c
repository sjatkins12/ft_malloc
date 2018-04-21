/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 19:49:45 by satkins           #+#    #+#             */
/*   Updated: 2018/03/17 15:50:38 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	*g_pages[3] = {0, 0, 0};

size_t		get_page_size(int p_size)
{
	static int	page = 0;
	size_t			len;

	if (!page)
		page = getpagesize();
	if (p_size == TINY_IND)
		len = (TINY + sizeof(t_meta)) * 101;
	else if (p_size == SMALL_IND)
		len = (SMALL + sizeof(t_meta)) * 101;
	else
		len = p_size + sizeof(t_meta);
	return (len + (len % page));
}

static void			*init_page(size_t p_size, size_t size)
{
	size_t			len;
	void			*page;
	t_meta			*head;

	len = get_page_size(p_size);
	page = NULL;
	if ((page = mmap(page, len, PROT_ALL, ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	head = page;
	head->mem_seg = page + sizeof(t_meta);
	head->len = size;
	head->start = page;
	head->used = 1;
	head->next_page = NULL;
	return (page);
}

static void			make_sum(t_meta *met_head)
{
	int				i;
	uint8_t			sum;

	met_head->chk_sum = 0;
	sum = 0;
	i = -1;
	while (++i < (int)sizeof(t_meta))
		sum += ((unsigned char *)met_head)[i];
	met_head->chk_sum = sum;	
}

static void			*iter_headers(void *head, size_t size, int p_size)
{
	t_meta			*met_head;
	t_meta			*prev;

	met_head = head;
	prev = NULL;
	while (1)
	{
		if (!met_head->used)
		{
			if (!met_head->mem_seg)
				met_head->mem_seg = (void *)met_head + sizeof(t_meta);
			if (!met_head->start)
				met_head->start = head;
			if (!met_head->p_index)
				met_head->p_index = p_size;
			if (prev && prev->next_page)
			{
				met_head->next_page = prev->next_page;
				prev->next_page = 0;
			}
			if (!met_head->len)
			{
				if (met_head->mem_seg + size < met_head->start + get_page_size(p_size))
				{
					met_head->len = size;
					met_head->used = 1;
					make_sum(met_head);
					return (met_head);
				}
				else if (met_head->next_page)
				{
					head = met_head->next_page;
					met_head = head;
					prev = NULL;
					continue ;
				}
				else
				{
					met_head->next_page = init_page(p_size, size);
					met_head = met_head->next_page;
					make_sum(met_head);					
					return (met_head);
				}
			}
			else if (size <= met_head->len)
			{
				met_head->used = 1;
				make_sum(met_head);
				return (met_head);
			}
		}
		prev = met_head;
		met_head = (void *)met_head + sizeof(t_meta) + met_head->len;
	}
}

static void			*find_space(size_t p_size, size_t size)
{
	void			*head;

	if (g_pages[p_size] == 0)
	{
		if ((head = init_page(p_size, size)) == NULL)
			return (NULL);
		((t_meta *)head)->len = size;
		g_pages[p_size] = head;
	}
	else if ((head = iter_headers(g_pages[p_size], size, p_size)) == NULL)
		return (NULL);
	return (((t_meta *)head)->mem_seg);
}

void	*malloc(size_t size)
{
	int				p_size;
	void			*tmp;
	t_meta			*head;

	if (size <= TINY)
		p_size = TINY_IND;
	else if (size <= SMALL)
		p_size = SMALL_IND;
	else if ((tmp = mmap(NULL, size + sizeof(t_meta), PROT_ALL, ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	else
	{
		head = tmp;
		head->len = size;
		head->mem_seg = tmp + sizeof(t_meta);
		head->p_index = LARGE_IND;
		head->start = head;
		head->used = 1;
		return (tmp);
	}
	if (!size)
		size = 1;
	return (find_space(p_size, size));
}
