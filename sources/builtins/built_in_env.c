/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:24:14 by bgresse           #+#    #+#             */
/*   Updated: 2023/02/28 12:13:59 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
