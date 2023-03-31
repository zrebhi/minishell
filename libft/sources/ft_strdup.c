/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 17:28:57 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/21 15:09:46 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include "../../includes/minishell.h"

char	*ft_strdup(const char *s1)
{
	char	*string;
	size_t	i;
	int		j;

	j = 0;
	i = ft_strlen(s1);
	string = ft_calloc(sizeof(char), i + 1);
	while (*s1)
	{
		string[j++] = *(s1++);
	}
	string[j] = '\0';
	return (string);
}
