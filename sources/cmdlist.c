/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:12:12 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/01 14:29:00 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Creates a new node in our linked list of commands.*/

void	ft_newnode(t_cmdlist **cmds);

/* Handles redirections for each command and 
creates a new command node everytime a '|' is found.
-5 means the redirection will go in a pipe. */

int		ft_error(char **parsed_line, int i)
{
	if (!parsed_line[i + 1])
		return (1);
	if ((!ft_strcmp(parsed_line[i + 1], ">") \
		||!ft_strcmp(parsed_line[i + 1], ">>") \
		|| !ft_strcmp(parsed_line[i + 1], "<") \
		|| !ft_strcmp(parsed_line[i + 1], "<<")) \
		|| !ft_strcmp(parsed_line[i + 1], "|") \
		|| !ft_strcmp(parsed_line[i + 1], "||"))
		return (2);
	return (0);
}

void	ft_print_error(char **parsed_line, int i)
{
	if (ft_error(parsed_line, i) == 1)
	{
		if (!ft_strcmp(parsed_line[i], "|") || !ft_strcmp(parsed_line[i], "||"))
		{
			ft_putstr_fd("syntax error near unexpected token '", 2);
			ft_putstr_fd(parsed_line[i], 2);
			ft_putstr_fd("\n'", 2);
		}
		else
			dprintf(2,"syntax error near unexpected token 'newline'\n");
	}
	if (ft_error(parsed_line, i) == 2)
	{
		if (!ft_strcmp(parsed_line[i], "|") || !ft_strcmp(parsed_line[i], "||"))
			dprintf(2,"syntax error near unexpected token '%s'\n", parsed_line[i]);
		else
			dprintf(2,"syntax error near unexpected token '%s'\n", parsed_line[i + 1]);
	}
}

void	ft_redirect_pipe(t_cmdlist **cmds)
{
	ft_newnode(cmds);
	if ((*cmds)->outfile == 1)
		(*cmds)->outfile = 0;
	*cmds = (*cmds)->next;
}

void	ft_redirect_infile(char **parsed_line, t_cmdlist *cmds, int i)
{
	if (!ft_error(parsed_line, i))
	{
		cmds->infile = open(parsed_line[i + 1], O_RDONLY);
		if (cmds->infile == -1)
			perror("open infile");
	}
}

void	ft_redirect_outfile(char **parsed_line, t_cmdlist *cmds, int i)
{
	if (!ft_error(parsed_line, i))
	{
		cmds->outfile = open(parsed_line[i + 1], \
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmds->outfile == -1)
			perror("open outfile");
	}	
}

void	ft_redirect_outfile_append(char **parsed_line, t_cmdlist *cmds, int i)
{
	if (!ft_error(parsed_line, i))
	{
		cmds->outfile = open(parsed_line[i + 1], O_WRONLY | O_CREAT | \
				O_APPEND, 0644);
		if (cmds->outfile == -1)
			perror("open outfile");
	}
}

static void	ft_input(t_cmdlist *cmds)
{
	size_t	length;
	char	*line;

	if (close(cmds->here_doc_pipe[0]) == -1)
		return (perror("close pipe"));
	while (1)
	{
		line = readline("> ");
		if (!line)
			return ;
		if (ft_strlen(line) - 1 > ft_strlen(cmds->limiter))
			length = ft_strlen(line) - 1;
		else
			length = ft_strlen(cmds->limiter);
		if (!ft_strncmp(line, cmds->limiter, length))
		{
			if (close(cmds->here_doc_pipe[1]) == -1)
				return (perror("close pipe"));
			exit(EXIT_SUCCESS) ;
		}
		ft_putstr_fd(line, cmds->here_doc_pipe[1]);
		ft_putstr_fd("\n", cmds->here_doc_pipe[1]);
		free(line);
	}
}

void	ft_heredoc(t_cmdlist *cmds)
{
	pid_t	here_doc_pid;

	if (pipe(cmds->here_doc_pipe) == -1)
		return (perror("heredoc pipe"));
	here_doc_pid = fork();
	if (here_doc_pid == -1)
		return (perror("heredoc fork"));
	if (here_doc_pid != 0 && close(cmds->here_doc_pipe[1]) == -1)
		return ((void)perror("close pipe"));
	if (here_doc_pid == 0)
		ft_input(cmds);
	waitpid(here_doc_pid, NULL, 0);
}

void	ft_redirect_heredoc(char **parsed_line, t_cmdlist *cmds, int i)
{
	if (!ft_error(parsed_line, i))
	{
		cmds->here_doc = 1;
		cmds->limiter = parsed_line[i + 1];
		ft_heredoc(cmds);
	}
}

void	ft_check_heredoc(char **parsed_line, t_cmdlist *cmds)
{
	int	i;

	i = -1;
	while (parsed_line[++i])
	{
		if (!strcmp(parsed_line[i], "|"))
			ft_redirect_pipe(&cmds);
		else if (!strcmp(parsed_line[i], "<<"))
			ft_redirect_heredoc(parsed_line, cmds, i);
	}
}

int	ft_redirection(char **parsed_line, t_cmdlist *cmds)
{
	int	i;

	i = -1;
	while (parsed_line[++i])
	{
		if (!strcmp(parsed_line[i], "|"))
			ft_redirect_pipe(&cmds);
		else if (!strcmp(parsed_line[i], "<"))
			ft_redirect_infile(parsed_line, cmds, i);
		else if (!strcmp(parsed_line[i], ">>"))
			ft_redirect_outfile_append(parsed_line, cmds, i);
		else if (!strcmp(parsed_line[i], ">"))
			ft_redirect_outfile(parsed_line, cmds, i);
		// if (ft_error(parsed_line, i))
		// {
		// 	ft_print_error(parsed_line, i);
		// 	return (0);
		// }
	}
	return (1);
}

/* For each node of our cmd list,
stores the command in a char ** that we can send directly to execve*/

void	ft_fullcmds(char **parsed_line, t_cmdlist *cmds)
{
	int			i;
	int			j;

	cmds->full_cmd = malloc(sizeof(char *) * 50);
	i = 0;
	j = 0;
	while (parsed_line[i])
	{
		if (!ft_strcmp(parsed_line[i], "|"))
		{
			cmds->full_cmd[j] = 0;
			cmds = cmds->next;
			cmds->full_cmd = malloc(sizeof(char *) * 50);
			j = 0;
			i++;
		}
		else if (!ft_strcmp(parsed_line[i], ">") ||!ft_strcmp(parsed_line[i], ">>") \
			|| !ft_strcmp(parsed_line[i], "<") || !ft_strcmp(parsed_line[i], "<<"))
		{
			i++;
			if (parsed_line[i])
				i++;
		}
		else
			cmds->full_cmd[j++] = parsed_line[i++];
	}
}

char	**ft_remove_quotes(char **strs);

t_cmdlist	*ft_cmdlist(char *cmd_line, t_minishell *data)
{
	char		**parsed_line;
	t_cmdlist	*cmds;

	cmds = 0;
	ft_newnode(&cmds);
	parsed_line = ft_split_tokens(cmd_line, "<|>");
	// (void)data;
	parsed_line = ft_expand_var(&data->head_env, parsed_line);
	parsed_line = ft_expand_path(&data->head_env, parsed_line);
	parsed_line = ft_remove_quotes(parsed_line);
	ft_check_heredoc(parsed_line, cmds);
	if (!ft_redirection(parsed_line, cmds))
		return (0);
	ft_fullcmds(parsed_line, cmds);
	return (cmds);
}
