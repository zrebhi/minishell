/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:13:52 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/21 17:43:41 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_check_error(char *string)
{
	bool	correct;
	int		i;

	correct = false;
	if (ft_isalpha(string[0]) || string[0] == '_')
		correct = true;
	i = 1;
	while (string[i])
	{
		if (!ft_isalnum(string[i]) && string[i] != '_')
			correct = false;
		i++;
	}
	if (!correct)
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd(string, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		g_global.g_status = 1;
	}	
}

static void	remove_env_node(t_env **head, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *head;
	prev = NULL;
	if (current != NULL && ft_strcmp(current->key, key) == 0)
	{
		*head = current->next;
		return ;
	}
	while (current != NULL && ft_strcmp(current->key, key) != 0)
	{
		prev = current;
		current = current->next;
	}
	if (current == NULL)
		return ;
	prev->next = current->next;
	return ;
}

void	ft_built_in_unset(t_env **head, char **key_list)
{
	size_t	i;
	t_env	*temp;

	i = 1;
	while (key_list[i])
	{
		temp = *head;
		while (temp)
		{
			if (ft_strcmp(temp->key, key_list[i]) == 0)
			{
				if (ft_strcmp(temp->key, "SHLVL") == 0)
					ft_set_env(head, "SHLVL", "0", 1);
				else
					remove_env_node(head, key_list[i]);
			}
			temp = temp->next;
		}
		if (!temp)
			ft_check_error(key_list[i]);
		i++;
	}
	temp = *head;
}
