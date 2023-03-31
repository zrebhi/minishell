/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:31:03 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/29 16:04:48 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_char(char const *s1, char c)
{
	char	*new_str;
	size_t	len1;
	size_t	i;

	if (!s1)
		return (NULL);
	len1 = ft_strlen(s1);
	new_str = ft_calloc(sizeof(char), len1 + 3);
	if (!new_str)
		exit(1);
	i = 0;
	while (i < len1)
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}

typedef struct s_quotes
{
	char			*string;
	bool			quote;
	bool			dquote;
}					t_quotes;

void	ft_quotes_handler(t_quotes *quotes, char **new_string, size_t *i)
{
	if (quotes->quote && quotes->string[(*i) + 1])
	{
		(*i)++;
		while (quotes->string[(*i)] && quotes->string[(*i)] != '\'')
			(*new_string) = ft_strjoin_char((*new_string), \
			quotes->string[(*i)++]);
		quotes->quote = !quotes->quote;
	}
	else if (quotes->dquote && quotes->string[(*i) + 1])
	{
		(*i)++;
		while (quotes->string[(*i)] && quotes->string[(*i)] != '\"')
			(*new_string) = ft_strjoin_char((*new_string), \
			quotes->string[(*i)++]);
		quotes->dquote = !quotes->dquote;
	}
	else if (quotes->string[(*i)] != '\\')
		(*new_string) = ft_strjoin_char((*new_string), quotes->string[(*i)]);
}

char	*ft_remove_quote_in_string(char *string, bool quote, bool dquote)
{
	char		*new_string;
	size_t		i;
	t_quotes	quotes;

	quotes.string = string;
	quotes.quote = quote;
	quotes.dquote = dquote;
	i = 0;
	new_string = ft_strdup("");
	while (quotes.string[i])
	{
		if ((!i || (i && quotes.string[i - 1] != '\\')) \
		&& quotes.string[i] == '\'')
			quotes.quote = !quotes.quote;
		if ((!i || (i && quotes.string[i - 1] != '\\')) \
		&& quotes.string[i] == '\"')
			quotes.dquote = !quotes.dquote;
		ft_quotes_handler(&quotes, &new_string, &i);
		i++;
	}
	return (new_string);
}

char	*ft_check_key(t_env *head, char *key)
{
	size_t	i;
	char	*temp_key;

	i = 0;
	while (key[i] && (ft_isalnum(key[i]) || key[i] == '_'))
		i++;
	temp_key = ft_substr(key, 0, i);
	while (head)
	{
		if (ft_strcmp(head->key, temp_key) == 0)
			return (ft_strdup(head->value));
		head = head->next;
	}
	return (ft_strdup(""));
}

int	ft_expand_check_heredoc(t_cmdlist *cmds)
{
	if (ft_strchr(cmds->limiter, '\'') || ft_strchr(cmds->limiter, '\"'))
	{
		cmds->limiter = ft_remove_quote_in_string(cmds->limiter, false, false);
		return (0);
	}
	return (1);
}
