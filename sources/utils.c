/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 18:05:56 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/31 15:20:48 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_another_minishell(t_minishell *data)
{
	if (ft_strstr(data->cmds->full_cmd[0], "minishell"))
		unplug_signals();
}

static int	is_a_directory(char *path)
{
	struct stat	path_stat;

	path_stat.st_mode = 0;
	stat(path, &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(" is a directory\n", 2);
		return (1);
	}
	return (0);
}

void	ft_dotslash(t_minishell *data)
{
	if (!ft_strncmp(data->cmds->full_cmd[0], "./", 2))
	{
		if (is_a_directory(data->cmds->full_cmd[0] + 2))
			return (exit(126));
		if (access(data->cmds->full_cmd[0], X_OK) == -1)
			return (perror(data->cmds->full_cmd[0]), exit(126));
		execve(data->cmds->full_cmd[0], data->cmds->full_cmd, data->envp);
		exit(0);
	}
}

char	**ft_strs_join(char **strs1, char **strs2)
{
	int		i;
	int		j;
	int		k;
	char	**strs_join;

	i = 0;
	while (strs1[i])
		i++;
	j = 0;
	while (strs2[j])
		j++;
	strs_join = ft_calloc(sizeof(char *), i + j + 1);
	k = 0;
	i = 0;
	while (strs1[i])
		strs_join[k++] = strs1[i++];
	j = 0;
	while (strs2[j])
		strs_join[k++] = strs2[j++];
	return (strs_join);
}
