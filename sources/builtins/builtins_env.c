/* ************************************************************************** */
/*	                                                                          */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 12:37:40 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/29 16:03:34 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_print_without_backslash(char *string)
{
	size_t	i;

	i = 0;
	while (string[i])
	{
		if (string[i + 1] && string[i] == '\\')
			i++;
		printf("%c", string[i]);
		i++;
	}
}

void	ft_built_in_env(char **envp)
{
	size_t	i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "SHLVL", 5) == 0)
		{
			if (ft_strchr(envp[i], '-'))
				printf("SHLVL=0\n");
			else
			{
				ft_print_without_backslash(envp[i]);
				printf("\n");
			}
		}
		else if (ft_strchr(envp[i], '='))
		{
			ft_print_without_backslash(envp[i]);
			printf("\n");
		}
	}
}
