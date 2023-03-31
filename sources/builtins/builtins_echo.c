/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:19:09 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/21 17:47:14 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_is_nflag(char *string)
{
	int		i;

	i = 0;
	if (string[i] != '-' || !string[1])
		return (0);
	else
		i++;
	while (string[i] && string[i] == 'n')
		i++;
	if (string[i])
		return (0);
	else
		return (1);
}

void	ft_built_in_echo(char **cmd)
{
	bool	n_flag;
	int		i;

	i = 1;
	n_flag = false;
	while (cmd[i] && ft_is_nflag(cmd[i]))
	{
		n_flag = true;
		i++;
	}
	while (cmd && cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (n_flag == false)
		printf("\n");
	g_global.g_status = 0;
}
