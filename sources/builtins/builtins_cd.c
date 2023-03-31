/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 00:53:23 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/21 15:28:04 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_set_env(t_env **head, char *key, char *value, bool equal)
{
	t_env	*temp;

	temp = *head;
	while (temp)
	{
		if (!ft_strcmp(temp->key, key))
		{
			temp->value = ft_strdup(value);
			temp->equal = equal;
			return ;
		}
		temp = temp->next;
	}
	*head = add_node(*head, key, value, equal);
}

int	ft_built_in_cd(t_env **head, char **full_cmd)
{
	char	cwd[PATH_MAX];

	if (!full_cmd[1])
	{
		if (ft_get_env(head, "HOME"))
			return (chdir(ft_get_env(head, "HOME")), 1);
		else
			return (ft_putstr_fd("cd : HOME not set\n", 2), 0);
	}
	if (chdir(full_cmd[1]) == -1)
		return (g_global.g_status = 1, perror("chdir()"), 0);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (ft_get_env(head, "PWD"))
			ft_set_env(head, "OLDPWD", ft_get_env(head, "PWD"), true);
		ft_set_env(head, "PWD", cwd, true);
		return (1);
	}
	else
		return (g_global.g_status = 1, perror("getcwd()"), 0);
}
