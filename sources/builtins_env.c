/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 12:37:40 by zrebhi            #+#    #+#             */
/*   Updated: 2023/02/28 13:28:06 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void ft_print_env_sorted(t_env *env)
{
    // Copie de la liste chaînée
    t_env *copy = env;
    int count = 0;
    while (copy != NULL)
    {
        count++;
        copy = copy->next;
    }

    t_env **array = malloc(count * sizeof(t_env *));
    if (array == NULL)
    {
        printf("Erreur d'allocation de mémoire\n");
        return;
    }

    copy = env;
    for (int i = 0; i < count; i++)
    {
        array[i] = copy;
        copy = copy->next;
    }

    // Tri à bulles
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(array[j]->key, array[j+1]->key) > 0)
            {
                t_env *temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }

    // Affichage des éléments triés
    for (int i = 0; i < count; i++)
    {
        printf("%s=%s\n", array[i]->key, array[i]->value);
    }

    // Libération de la mémoire allouée
    free(array);
}

void    ft_built_in_export(t_env **head, char **full_cmd)
{
    size_t  i;

    i = 1;
    if (!full_cmd[1])
        ft_print_env_sorted(*head);
    while (full_cmd[i])
    {
    	char *equal = ft_strchr(full_cmd[i], '=');
    	if (equal)
    	{
      		*equal = '\0';
      		*head = add_node(*head, full_cmd[i], equal + 1);
    	}
        else
            *head = add_node(*head, full_cmd[i], "''");
        i++;
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
		free(current->key);
		free(current->value);
		free(current);
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
	return (free(current->key), free(current->value), free(current));
}

void	ft_built_in_unset(t_env **head, char *key)
{
	size_t	i;
	t_env	*temp;
	char	**key_list;

	i = 1;
	key_list = ft_split(key, ' ');
	while (key_list[i])
	{
		temp = *head;
		while (temp)
		{
			if (ft_strcmp(temp->key, key_list[i]) == 0)
				remove_env_node(head, key_list[i]);
			temp = temp->next;
		}
		i++;
	}
}
