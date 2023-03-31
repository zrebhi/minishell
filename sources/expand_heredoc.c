/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 16:15:55 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/27 14:40:50 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

typedef struct s_expand
{
	char	**strs_cmd;
	int		i;
	int		j;
}			t_expand;

static void	handle_dollar_heredoc(char **new_cmds, int *i, char *cmds, \
								t_env *head)
{
	char	*expander;

	*i = *i + 1;
	expander = ft_check_key(head, cmds + *i);
	*new_cmds = ft_strjoin(*new_cmds, expander);
	if (!ft_isdigit(cmds[(*i)]))
	{
		while (cmds[(*i) + 1] && \
			(ft_isalnum(cmds[(*i) + 1]) || cmds[(*i) + 1] == '_'))
			(*i) = (*i) + 1;
	}
}

char	*ft_expand_var_heredoc(t_env *head, char *cmds)
{
	int		i;
	char	*new_cmds;

	i = 0;
	new_cmds = ft_strdup("");
	while (cmds[i])
	{
		if (cmds[i] == '$' && cmds[i + 1] && ft_isalnum(cmds[i + 1]))
			handle_dollar_heredoc(&new_cmds, &i, cmds, head);
		else if (cmds[i] == '$' && cmds[i + 1] == '?')
		{
			i++;
			new_cmds = ft_strjoin(new_cmds, ft_itoa(g_global.g_status));
		}
		else
			new_cmds = ft_strjoin_char(new_cmds, cmds[i]);
		i++;
	}
	return (new_cmds);
}
