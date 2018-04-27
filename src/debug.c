/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/26 10:28:10 by satkins           #+#    #+#             */
/*   Updated: 2018/04/27 10:43:59 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

#ifdef DEBUG

void	page_creation_debug(void *page, size_t len)
{
	write(1, "Allocation: 0x", 14);
	ft_putul((size_t)page);
	write(1, " - 0x", 5);
	ft_putul((size_t)page + len);
	write(1, "\n", 1);
}
#endif

#ifndef DEBUG

void	page_creation_debug(__attribute__((unused))void *page,
	__attribute__((unused))size_t len)
{
	;
}
#endif