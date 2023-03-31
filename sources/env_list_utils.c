/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:18:32 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/20 18:07:57 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*new_node(char *key, char *value, bool equal)
{
	t_env	*node;

	node = ft_calloc(sizeof(t_env), 1);
	node->key = ft_strndup(key, ft_strlen(key));
	node->value = ft_strndup(value, ft_strlen(value));
	node->equal = equal;
	return (node);
}

t_env	*add_node(t_env *head, char *key, char *value, bool equal)
{
	t_env	*node;

	node = new_node(key, value, equal);
	node->next = head;
	return (node);
}

int	ft_list_size(t_env *head)
{
	int	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

char	**ft_refresh_envp(t_env *head)
{
	size_t	env_len;
	char	**env;
	size_t	i;

	if (!head)
		return (NULL);
	env_len = ft_list_size(head);
	env = ft_calloc(sizeof(char *), (env_len + 1));
	i = 0;
	while (head)
	{
		if (head->equal)
			env[i] = ft_strjoin(head->key, ft_strjoin("=", head->value));
		else
			env[i] = head->key;
		i++;
		head = head->next;
	}
	env[i] = NULL;
	return (env);
}
