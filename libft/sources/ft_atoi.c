/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 11:54:37 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/15 16:14:42 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include "../../includes/minishell.h"

static int	ft_whitesapce(const char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-')
		i++;
	else if (str[i] == '+')
		i++;
	return (i);
}

static int	ft_sign(const char *str)
{
	int	sign;
	int	i;

	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = sign * -1;
		return (sign);
	}
	return (sign);
}

long	ft_atoi(const char *str)
{
	long long int	finale;
	int				i;
	int				sign;

	sign = ft_sign(str);
	finale = 0;
	i = ft_whitesapce(str);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (finale != ((finale * 10 + (sign * (str[i] - '0'))) / 10))
			return (2147483648);
		finale = finale * 10 + ((str[i] - '0') * sign);
		i++;
	}
	return ((int)finale);
}
