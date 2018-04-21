/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 16:47:52 by satkins           #+#    #+#             */
/*   Updated: 2018/03/17 15:57:43 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC

# define FT_MALLOC

# include "libft.h"
# include <sys/mman.h>
# include <pthread.h>
extern void				*g_pages[3];


# define TINY 128
# define SMALL 2048
# define PROT_ALL (PROT_READ | PROT_WRITE | PROT_EXEC)
# define ANON (MAP_ANONYMOUS | MAP_PRIVATE | MAP_NOCACHE)
# define TINY_IND 0
# define SMALL_IND 1
# define LARGE_IND 2

typedef struct		s_meta
{
	void			*mem_seg;
	void			*start;
	size_t			len;
	uint8_t			used;
	uint8_t			p_index;
	void			*next_page;
	uint8_t			chk_sum;
}					t_meta;

void				*malloc(size_t size);
void				free(void *ptr);
size_t				get_page_size(int p_size);
void				*realloc(void *ptr, size_t size);
int					validate_csum(t_meta *met_head);

#endif