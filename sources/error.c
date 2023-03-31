/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:22:37 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/20 18:07:41 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* The 4 following functions handle the error message of bash
for invalid redirections. */

int	ft_error_redirect_out(char **parsed_line, int i)
{
	if (!ft_strcmp(parsed_line[i], ">"))
	{
		if (!parsed_line[i + 1])
			return (1);
		if (!ft_strcmp(parsed_line[i + 1], "|"))
			i++;
		if (!parsed_line[i + 1])
			return (1);
		if (!ft_strcmp(parsed_line[i + 1], ">") \
			|| !ft_strcmp(parsed_line[i + 1], ">>")
			|| !ft_strcmp(parsed_line[i + 1], "<") \
			|| !ft_strcmp(parsed_line[i + 1], "<<"))
			return (2);
	}
	return (0);
}

int	ft_error(char **parsed_line, int i)
{
	if (!ft_strcmp(parsed_line[i], "||"))
		return (4);
	if (ft_error_redirect_out(parsed_line, i))
		return (ft_error_redirect_out(parsed_line, i));
	if ((!ft_strcmp(parsed_line[i], ">>") \
		|| !ft_strcmp(parsed_line[i], "<") \
		|| !ft_strcmp(parsed_line[i], "<<")))
	{
		if (!parsed_line[i + 1])
			return (1);
		if (!ft_strcmp(parsed_line[i + 1], ">") \
			||!ft_strcmp(parsed_line[i + 1], ">>") \
			|| !ft_strcmp(parsed_line[i + 1], "<") \
			|| !ft_strcmp(parsed_line[i + 1], "<<") \
			|| !ft_strcmp(parsed_line[i + 1], "|"))
			return (2);
	}
	if (!ft_strcmp(parsed_line[i], "|"))
	{
		if (!parsed_line[i + 1] || i == 0 || \
			!ft_strcmp(parsed_line[i + 1], "|"))
			return (3);
	}
	return (0);
}

void	ft_syntax_error(char **parsed_line, int i)
{
	if (i && !ft_strcmp(parsed_line[i - 1], ">") \
		&& !ft_strcmp(parsed_line[i], "|"))
		i++;
	ft_putstr_fd("syntax error near unexpected token '", 2);
	ft_putstr_fd(parsed_line[i], 2);
	ft_putstr_fd("'\n", 2);
}

void	ft_print_error(char **parsed_line, int i)
{
	if (ft_error(parsed_line, i) == 1)
		ft_putstr_fd("syntax error near unexpected token 'newline'\n", 2);
	if (ft_error(parsed_line, i) == 2)
		ft_syntax_error(parsed_line, i + 1);
	if (ft_error(parsed_line, i) == 3)
		ft_syntax_error(parsed_line, i);
	if (ft_error(parsed_line, i) == 4)
		ft_putstr_fd("minishell : '||' not supported\n", 2);
}
