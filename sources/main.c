/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:02:59 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/07 15:04:52 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	data_init(int argc, char **argv, char **envp, t_minishell *data)
{
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	ft_parse_env(&data->head_env, data->envp);
	data->paths = ft_pathfinder(&data->head_env);
	g_status = 0;
}

static char	*get_prompt(t_env *head, char *key)
{
	t_env	*temp;

	temp = head;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (ft_strjoin(ft_strjoin(""GREEN"➜  "CYAN"", temp->value),
					""PURPLE"@minishell > "RESET""));
		temp = temp->next;
	}
	return (""GREEN"➜  "CYAN"guest"PURPLE"@minishell > "RESET"");
}

void	ft_special_builtins(t_minishell *data);
void	set_interactive_signals(void);

void	ft_prompt(t_minishell *data)
{
	char		*buffer;
	int			pid;
	char		*prompt;

	prompt = get_prompt(data->head_env, "USER");
	while (1)
	{
		set_interactive_signals();
		buffer = readline(prompt);
		if (!buffer)
			break ;
		if (!*buffer)
			continue ;
		add_history(buffer);
		data->cmds = ft_cmdlist(buffer, data);
		// ft_print_cmdlist(data->cmds);
		if (data->cmds)
		{
			unplug_signals();
			ft_special_builtins(data);
			pid = fork();
			if (pid == 0)
				pipex(data);
			waitpid(pid, &data->status, 0);
			if (!g_status)
				g_status = WEXITSTATUS(g_status);
			while (data->cmds)
			{
				if (data->cmds->here_doc)
					if (close(data->cmds->here_doc_pipe[0]) == -1)
						perror("close pipe");
				data->cmds = data->cmds->next;
			}
			while (data->cmds)
			{
				if (data->cmds->outfile > 1)
					if (close(data->cmds->outfile) == -1)
						perror("close outfile");
				if (data->cmds->infile > 1)
					if (close(data->cmds->infile) == -1)
						perror("close infile");
			}
		}
	}
	printf("exit\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	data = malloc(sizeof(t_minishell));
	data_init(argc, argv, envp, data);
	ft_prompt(data);
	return (g_status);
}
