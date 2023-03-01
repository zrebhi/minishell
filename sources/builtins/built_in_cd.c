/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:24:08 by bgresse           #+#    #+#             */
/*   Updated: 2023/02/28 12:21:32 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_built_in_cd(char **full_cmd)
{
	if (chdir(full_cmd[1]) == -1)
		perror("chdir()");
}

void	ft_built_in_exit(void)
{
	exit(0);
}

void	ft_built_in_env(t_env **head)
{
	t_env	*temp;

	temp = *head;
	while (temp)
	{
		printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}

void	ft_built_in_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd()");
}