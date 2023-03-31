/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 13:01:10 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/21 15:05:28 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include "../../includes/minishell.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size && SIZE_MAX / size < count)
		return (NULL);
	ptr = ft_free_malloc(g_global.m_free, count * size);
	if (!ptr)
	{
		ft_free(g_global.m_free);
		exit(1);
	}
	ft_bzero(ptr, size * count);
	return (ptr);
}
