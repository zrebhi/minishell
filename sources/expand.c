/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 19:18:09 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/29 14:10:21 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 19:18:09 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/21 17:07:58 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_char(char const *s1, char c);
char	*ft_check_key(t_env *head, char *key);

static void	ft_signal_join(char **new_cmds, int *i)
{
	*i = *i + 1;
	*new_cmds = ft_strjoin(*new_cmds, ft_itoa(g_global.g_status));
}

static void	handle_dollar(char **new_cmds, int *i, char *cmds, t_env **head)
{
	*i = *i + 1;
	*new_cmds = ft_strjoin(*new_cmds, ft_check_key(*head, cmds + *i));
	if (!ft_isdigit(cmds[(*i)]))
	{
		while (cmds[(*i) + 1] && \
			(ft_isalnum(cmds[(*i) + 1]) || cmds[(*i) + 1] == '_'))
			(*i) = (*i) + 1;
	}
}

char	*ft_expand_var(t_env **head, char *cmds, bool dq, bool sq)
{
	int		i;
	char	*new_cmds;

	i = 0;
	new_cmds = ft_strdup("");
	while (cmds[i])
	{
		if (cmds[i] == '\"')
			dq = !dq;
		if (cmds[i] == '\'' && !dq)
			sq = !sq;
		if (sq && !dq && cmds[i + 1])
			while (cmds[i] && cmds[i + 1] != '\'')
				new_cmds = ft_strjoin_char(new_cmds, cmds[i++]);
		if (cmds[i] == '$' && (dq || (!dq && !sq)) \
			&& cmds[i + 1] && ft_isalnum(cmds[i + 1]))
			handle_dollar(&new_cmds, &i, cmds, head);
		else if (cmds[i] == '$' && (dq || (!dq && !sq)) && cmds[i + 1] == '?')
			ft_signal_join(&new_cmds, &i);
		else
			new_cmds = ft_strjoin_char(new_cmds, cmds[i]);
		i++;
	}
	return (new_cmds);
}

char	**ft_remove_quotes(char **cmds)
{
	size_t	i;

	i = 0;
	while (cmds[i])
	{
		if (ft_strchr(cmds[i], '\'') || ft_strchr(cmds[i], '\"'))
			cmds[i] = ft_remove_quote_in_string(cmds[i], false, false);
		i++;
	}
	return (cmds);
}
