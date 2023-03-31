/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:17:49 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/31 15:26:25 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_builtins(t_minishell *data);

/* Handles the executions of the commands and returns the corresponding error
if the command was not found. */

char	**ft_remove_quotes(char **cmds);

void	ft_dotslash(t_minishell *data);

void	ft_exec(t_minishell *data)
{
	int		i;
	char	*cmd;

	ft_dotslash(data);
	data->paths = ft_pathfinder(&data->head_env);
	i = -1;
	execve(data->cmds->full_cmd[0], data->cmds->full_cmd, data->envp);
	if (data->paths)
	{
		while (data->paths[++i])
		{
			cmd = ft_strjoin(data->paths[i], data->cmds->full_cmd[0]);
			if (!cmd)
				return ;
			execve(cmd, data->cmds->full_cmd, data->envp);
		}
	}
	ft_putstr_fd("command not found: ", 2);
	if (data->cmds->full_cmd[0])
		ft_putstr_fd(data->cmds->full_cmd[0], 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

/* Handles everything related to children processes. */

void	ft_incubator(t_minishell *data)
{
	if (close(data->end[0]) == -1)
		return (perror("close pipe"));
	if (data->cmds->infile > 0 && !data->cmds->here_doc)
		if (dup2(data->cmds->infile, STDIN_FILENO) == -1)
			return ;
	if (data->cmds->outfile > 1)
		if (dup2(data->cmds->outfile, STDOUT_FILENO) == -1)
			return ;
	if (data->cmds->outfile == 0)
		if (dup2(data->end[1], STDOUT_FILENO) == -1)
			return (perror("dup2 end[1]"));
	if (close(data->end[1]) == -1)
		return ((void)perror("close pipe"));
	if (ft_builtins(data))
		exit(g_global.g_status);
	ft_exec(data);
}

void	pipex_heredoc(t_minishell *data)
{
	if (data->cmds->here_doc)
	{
		if (data->cmds->full_cmd[0])
		{
			if (dup2(data->cmds->here_doc_pipe[0], STDIN_FILENO) == -1)
				return (perror("dup2 heredoc"));
		}
		if (close(data->cmds->here_doc_pipe[0]) == -1)
			return (perror("close pipe heredoc"));
	}
}

/* Creates a child process for every command and links them together
with pipes */

void	set_exec_signals(void);

void	ft_another_minishell(t_minishell *data);

void	pipex_commands(t_minishell *data)
{
	while (data->cmds)
	{
		pipex_heredoc(data);
		if (data->cmds->error || !data->cmds->full_cmd[0])
			data->cmds = data->cmds->next;
		else
		{
			if (pipe(data->end) == -1)
				return ((void)perror("pipe"));
			data->cmds->cmd_pid = fork();
			if (data->cmds->cmd_pid == -1)
				return ((void)perror("Fork"));
			if (data->cmds->cmd_pid == 0)
				ft_incubator(data);
			ft_another_minishell(data);
			if (close(data->end[1]) == -1)
				return ((void)perror("close pipe"));
			if (dup2(data->end[0], STDIN_FILENO) == -1)
				return ((void)perror("dup2 end[0]"));
			if (close(data->end[0]) == -1)
				return ((void)perror("close pipe"));
			data->cmds = data->cmds->next;
		}	
	}
}

/* Runs our commands with pipex_commands, then closes our remaining fds
and waits for children processes to be done */

void	pipex(t_minishell *data)
{
	t_cmdlist	*head;

	head = data->cmds;
	set_exec_signals();
	pipex_commands(data);
	data->cmds = head;
	while (data->cmds)
	{
		if (data->cmds->error)
		{
			g_global.g_status = data->cmds->error;
			data->cmds = data->cmds->next;
			continue ;
		}
		waitpid(data->cmds->cmd_pid, &data->status, 0);
		g_global.g_status = WEXITSTATUS(data->status);
		data->cmds = data->cmds->next;
	}
	return (ft_free(g_global.m_free), exit (g_global.g_status));
}
