/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:02:59 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/29 16:11:19 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_special_builtins(t_minishell *data);
void	set_interactive_signals(void);
char	**ft_refresh_envp(t_env *head);
char	*ft_remove_quote_in_string(char *string, bool quote, bool dquote);

void	data_init(int argc, char **argv, char **envp, t_minishell *data)
{
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	ft_parse_env(&data->head_env, data->envp);
	data->paths = ft_pathfinder(&data->head_env);
	g_global.g_status = 0;
}

void	ft_execution(t_minishell *data)
{
	int			pid;

	unplug_signals();
	ft_special_builtins(data);
	pid = fork();
	if (pid == 0)
		pipex(data);
	waitpid(pid, &data->status, 0);
	g_global.g_status = WEXITSTATUS(data->status);
	while (data->cmds)
	{
		if (data->cmds->here_doc && g_global.g_status != 130 && \
			close(data->cmds->here_doc_pipe[0]) == -1)
			perror("close heredoc");
		if (data->cmds->outfile > 1 && close(data->cmds->outfile) == -1)
			perror("close outfile");
		if (data->cmds->infile > 1 && close(data->cmds->infile) == -1)
			perror("close infile main");
		data->cmds = data->cmds->next;
	}
}

void	ft_prompt(t_minishell *data)
{
	char		*buffer;

	while (1)
	{
		data->envp = ft_refresh_envp(data->head_env);
		set_interactive_signals();
		buffer = readline("Oui Michelle ? > ");
		if (!buffer)
			break ;
		if (!*buffer)
			continue ;
		add_history(buffer);
		data->cmds = ft_cmdlist(buffer, data);
		free(buffer);
		if (data->cmds)
			ft_execution(data);
	}	
	printf("exit\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	g_global.m_free = ft_free_init();
	data = ft_calloc(sizeof(t_minishell), 1);
	if (!data)
		return (1);
	data_init(argc, argv, envp, data);
	ft_prompt(data);
	return (ft_free(g_global.m_free), g_global.g_status);
}
