/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satkins <satkins@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 16:47:52 by satkins           #+#    #+#             */
/*   Updated: 2018/04/26 10:30:08 by satkins          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC

# define FT_MALLOC

# include "libft.h"
# include <sys/mman.h>
# include <pthread.h>
# include <sys/resource.h>

# define TINY 128
# define SMALL 2048

# define PROT_ALL (PROT_READ | PROT_WRITE | PROT_EXEC)
# define ANON (MAP_ANONYMOUS | MAP_PRIVATE | MAP_NOCACHE)

extern pthread_rwlock_t	g_rwlock[3];

extern void				*g_pages[5];
# define TINY_IND 0
# define SMALL_IND 1
# define LARGE_IND 2
# define MALLOC_CHECK_MAX 3
# define MALLOC_CHECK_IND 4

# define GARBAGE_INTERVAL 2

# define N_HEAD ((t_meta *)((void *)iter_head + sizeof(t_meta) + space))->next_page
# define LARGE_PAGE size + sizeof(t_meta) * 2 + len - (size + sizeof(t_meta) * 2) % len

typedef struct			s_meta
{
	void				*mem_seg;
	void				*start;
	size_t				len;
	uint8_t				used;
	int					p_index;
	void				*next_page;
	int					chk_sum;
}						t_meta;

void					*malloc(size_t size);
void					free(void *ptr);
size_t					get_page_size(int p_size);
void					*realloc(void *ptr, size_t size);
t_meta					*init_page(size_t p_size, size_t size);
void					*set_meta(t_meta *meta, size_t size, int p_size);
int						validate_addr(void *ptr);
size_t					check_contiguous(t_meta *start_head, size_t size, int p_size);
void					*iter_headers(void *head, size_t size, int p_size);
void					show_alloc_mem(void);
void					*background_cleaner(__attribute__((unused))void *vargp);

/*
**	Check_Sum
*/
int						make_sum(t_meta *met_head);
int						validate_csum(t_meta *met_head);

/*
**	Debug
*/
void					page_creation_debug(void *page, size_t len);

#endif
