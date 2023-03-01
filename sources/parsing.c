/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:18:32 by zrebhi            #+#    #+#             */
/*   Updated: 2023/02/24 17:58:04 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Retrieves the different paths from the environment */

t_env	*new_node(char *key, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	node->key = ft_strndup(key, ft_strlen(key));
	node->value = ft_strndup(value, ft_strlen(value));
	return (node);
}

t_env	*add_node(t_env *head, char *key, char *value)
{
	t_env	*node;

	node = new_node(key, value);
	node->next = head;
	return (node);
}


char	**ft_strsdup(char **strs)
{
	char	**strs_dup;
	int		i;

	i = 0;
	while (strs[i])
		i++;
	strs_dup = malloc(sizeof(char *) * (i + 2));
	if (!strs)
		return (0);
	i = -1;
	while (strs[++i])
		strs_dup[i] = strs[i];
	strs_dup[i] = "salut";
	i++;
	strs_dup[i] = 0;
	return (strs_dup);
}

int	ft_list_size(t_env *head)
{
	int count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

char	**ft_join_envp(t_env **head)
{
	t_env	*current;
	char	**envp;
	size_t	i;

	i = 0;
	current = *head;
	envp = malloc(sizeof(char *) * ft_list_size(*head));
	while (current)
	{
		envp[i++] = ft_strjoin(current->key, ft_strjoin("=", current->value));
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	ft_parse_env(t_env **head, char **envp)
{
	size_t	i;
	char	*equal;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			key = ft_strndup(envp[i], equal - envp[i]);
			value = ft_strdup(equal + 1);
			*head = add_node(*head, key, value);
			free(key);
		}
		i++;
	}
}

char	*ft_get_env(t_env **head_env, char *key)
{
	t_env	*temp;

	temp = *head_env;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

char	**ft_pathfinder(t_env **head)
{
	char	*paths_line;
	char	**paths;

	paths_line = ft_get_env(head, "PATH");
	paths = ft_split_slash(paths_line, ':');
	return (paths);
}