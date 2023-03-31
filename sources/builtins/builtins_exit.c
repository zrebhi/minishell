/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 00:53:31 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/21 17:30:39 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit_code(t_minishell *data, int in_pipe)
{
	if (ft_atoi(data->cmds->full_cmd[1]) < 2147483648)
		g_global.g_status = ft_atoi(data->cmds->full_cmd[1]);
	else
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(data->cmds->full_cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		g_global.g_status = 2;
	}
	if (!in_pipe)
		return (ft_free(g_global.m_free), exit(g_global.g_status));
}

void	ft_numarg_check(t_minishell *data)
{
	int		i;

	i = -1;
	while (data->cmds->full_cmd[1][++i])
	{
		if (i == 0 && (data->cmds->full_cmd[1][i] == '-' \
		|| data->cmds->full_cmd[1][i] == '+'))
			i++;
		if (data->cmds->full_cmd[1][i] < '0'
		|| data->cmds->full_cmd[1][i] > '9')
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(data->cmds->full_cmd[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			return (ft_free(g_global.m_free), exit(2));
		}
	}
}

void	ft_built_in_exit(t_minishell *data, int in_pipe)
{
	printf("exit\n");
	if (!data->cmds->full_cmd[1] || !*data->cmds->full_cmd[1])
		return (ft_free(g_global.m_free), exit(g_global.g_status));
	ft_numarg_check(data);
	if (data->cmds->full_cmd[2])
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(": too many arguments\n", 2);
		data->cmds = 0;
		g_global.g_status = 1;
	}
	else
		ft_exit_code(data, in_pipe);
}
