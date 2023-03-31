/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:52:56 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/29 16:39:36 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_heredoc_signals(void);

void	heredoc_child_init(t_cmdlist *cmds)
{
	set_heredoc_signals();
	if (close(cmds->here_doc_pipe[0]) == -1)
		return (perror("close pipe"));
	if (dup2(cmds->here_doc_pipe[1], 2) == -1)
		return (perror("close dup2"));
	if (close(cmds->here_doc_pipe[1]) == -1)
		return (perror("close pipe"));
}

char	*ft_remove_quote_in_string(char *string, bool quote, bool dquote);
int		ft_expand_check_heredoc(t_cmdlist *cmds);
char	*ft_expand_var_heredoc(t_env **head, char *cmds);

static void	ft_input(t_cmdlist *cmds, t_env **head_env, int expand)
{
	char	*line;
	char	*buffer;

	heredoc_child_init(cmds);
	buffer = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("warning: here-document delimited by end-of-file (wanted `%s')\
			\n", cmds->limiter);
			break ;
		}
		if (!ft_strcmp(line, cmds->limiter))
			break ;
		if (expand)
			line = ft_expand_var_heredoc(head_env, line);
		line = ft_strjoin(line, "\n");
		buffer = ft_strjoin(buffer, line);
	}
	ft_free_remove(g_global.m_free, line);
	if (buffer)
		write(2, buffer, ft_strlen(buffer));
	return (ft_free(g_global.m_free), exit(0));
}

void	ft_heredoc(t_cmdlist *cmds, t_env **head)
{
	pid_t	here_doc_pid;
	int		status;

	unplug_signals();
	if (pipe(cmds->here_doc_pipe) == -1)
		return (perror("heredoc pipe"));
	here_doc_pid = fork();
	if (here_doc_pid == -1)
		return (perror("heredoc fork"));
	if (here_doc_pid == 0)
		ft_input(cmds, head, ft_expand_check_heredoc(cmds));
	if (close(cmds->here_doc_pipe[1]) == -1)
		return (perror("close pipe"));
	waitpid(here_doc_pid, &status, 0);
	g_global.g_status = WEXITSTATUS(status);
	if (g_global.g_status)
	{
		cmds->error = g_global.g_status;
		if (close(cmds->here_doc_pipe[0]) == -1)
			return (perror("close pipe"));
	}
}

void	ft_redirect_pipe(t_cmdlist **cmds, char **parsed_line, int i);
void	ft_redirect_infile(char **parsed_line, t_cmdlist *cmds, int i);
char	**ft_remove_quotes(char **strs);
void	ft_print_error(char **parsed_line, int i);
int		ft_error(char **parsed_line, int i);

void	ft_redirect_heredoc(t_cmdlist *cmds, char **parsed_line, \
							int i, t_env **head)
{
	if (cmds->infile && close(cmds->infile) == -1)
		perror("pipe multi-heredoc");
	cmds->infile = 0;
	if (cmds->here_doc && close(cmds->here_doc_pipe[0]) == -1)
		perror("pipe multi-heredoc");
	if (!cmds->here_doc)
		cmds->here_doc = 1;
	cmds->limiter = parsed_line[i + 1];
	ft_heredoc(cmds, head);
}

int	ft_check_heredoc(char **parsed_line, t_cmdlist *cmds, t_env **head, int i)
{
	while (parsed_line[++i])
	{
		if (ft_error(parsed_line, i))
		{
			ft_print_error(parsed_line, i);
			g_global.g_status = 2;
			return (0);
		}
		if (!ft_strcmp(parsed_line[i], ">") && \
			parsed_line[i + 1] && !ft_strcmp(parsed_line[i + 1], "|"))
			i++;
		else if (!ft_strcmp(parsed_line[i], "|"))
			ft_redirect_pipe(&cmds, parsed_line, i);
		else if (!ft_strcmp(parsed_line[i], "<<"))
			ft_redirect_heredoc(cmds, parsed_line, i, head);
		else if (!ft_strcmp(parsed_line[i], "<"))
		{
			if (cmds->here_doc && close(cmds->here_doc_pipe[0]) == -1)
				perror("close infile");
			cmds->here_doc = 0;
		}
	}
	return (1);
}
