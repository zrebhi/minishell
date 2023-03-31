/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:12:12 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/29 17:47:48 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Creates a new node in our linked list of commands.*/

void	ft_newnode(t_cmdlist **cmds);

int	ft_cmd_size(char **parsed_line, int i)
{
	int	size;

	size = 1;
	if (!ft_strcmp(parsed_line[i], "|"))
		i++;
	while (parsed_line[i])
	{
		if (!ft_strcmp(parsed_line[i], "|") && \
			(!i || (i && ft_strcmp(parsed_line[i - 1], ">"))))
			break ;
		size++;
		i++;
	}
	return (size);
}

/* For each node of our cmd list,
stores the command in a char ** that we can send directly to execve.
Parameters int i and int j must be set to 0 on function call to save lines. */

void	ft_new_cmd(t_cmdlist **cmds, int *i, int *j, char **parsed_line)
{
	(*cmds)->full_cmd[(*j)] = 0;
	(*cmds) = (*cmds)->next;
	(*cmds)->full_cmd = ft_calloc(sizeof(char *), \
	ft_cmd_size(parsed_line, (*i)));
	(*j) = 0;
	(*i)++;
}

void	ft_fullcmds(char **parsed_line, t_cmdlist *cmds, int i, int j)
{
	cmds->full_cmd = ft_calloc(sizeof(char *), ft_cmd_size(parsed_line, i));
	while (parsed_line[i])
	{
		if (!ft_strcmp(parsed_line[i], ">"))
		{
			i++;
			if (parsed_line[i] && !ft_strcmp(parsed_line[i], "|"))
				i++;
			if (parsed_line[i])
				i++;
		}
		else if (!ft_strcmp(parsed_line[i], "|"))
			ft_new_cmd(&cmds, &i, &j, parsed_line);
		else if (!ft_strcmp(parsed_line[i], ">>") \
			|| !ft_strcmp(parsed_line[i], "<") \
			|| !ft_strcmp(parsed_line[i], "<<"))
		{
			i++;
			if (parsed_line[i])
				i++;
		}
		else
			cmds->full_cmd[j++] = parsed_line[i++];
	}
}

/* ft_cmdlist creates a linked list containing all the commands 
in the input line and useful informations for each of them. */

int		ft_check_heredoc(char **parsed_line, t_cmdlist *cmds, \
						t_env *head, int i);
int		ft_redirection(char **parsed_line, t_cmdlist *cmds);
char	**ft_strs_join(char **strs1, char **strs2);

char	**ft_expander(char **parsed_line, t_minishell *data)
{
	int			i;
	char		*cmd_line;

	cmd_line = ft_calloc(1, 1);
	i = -1;
	while (parsed_line[++i])
		parsed_line[i] = ft_expand_var(&data->head_env, parsed_line[i], 0, 0);
	i = -1;
	while (parsed_line[++i + 1])
	{
		cmd_line = ft_strjoin(cmd_line, parsed_line[i]);
		cmd_line = ft_strjoin(cmd_line, " ");
	}
	cmd_line = ft_strjoin(cmd_line, parsed_line[i]);
	parsed_line = ft_split_tokens(cmd_line, "");
	return (parsed_line);
}

t_cmdlist	*ft_cmdlist(char *cmd_line, t_minishell *data)
{
	char		**parsed_line;
	t_cmdlist	*cmds;
	t_cmdlist	*temp;

	cmds = 0;
	ft_newnode(&cmds);
	parsed_line = ft_split_tokens(cmd_line, "<|>");
	if (!ft_check_heredoc(parsed_line, cmds, data->head_env, -1))
		return (0);
	if (!ft_redirection(parsed_line, cmds))
		return (0);
	ft_fullcmds(parsed_line, cmds, 0, 0);
	temp = cmds;
	while (temp)
	{
		if (temp->full_cmd[0])
		{
			temp->full_cmd = ft_expander(temp->full_cmd, data);
			if (ft_strcmp("export", temp->full_cmd[0]))
				temp->full_cmd = ft_remove_quotes(temp->full_cmd);
		}
		temp = temp->next;
	}
	return (cmds);
}
