/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:12:58 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/29 16:01:53 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <stdio.h>
# include <termios.h>
# include <fcntl.h>
# include <limits.h>

# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif

# define STDOUT	1
# define SQUOTE 39
# define DQUOTE 34
# define DOLLAR 36
# define TILDE 126

typedef int	t_pid;

# define YELLOW "\033[1;33m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define BLUE "\033[1;34m"
# define PURPLE "\033[1;35m"
# define CYAN "\033[1;36m"
# define WHITE "\033[1;37m"
# define RESET "\033[0m"

typedef struct s_cmdlist
{
	char				**full_cmd;
	int					infile;
	int					outfile;
	int					error;
	int					export;
	int					here_doc;
	int					here_doc_pipe[2];
	char				*limiter;
	t_pid				cmd_pid;
	struct s_cmdlist	*next;
}						t_cmdlist;

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			equal;
	struct s_env	*next;
}					t_env;

typedef struct s_minishell
{
	char		**paths;

	t_cmdlist	*cmds;
	t_env		*head_env;
	t_m_free	*m_free;

	char		**envp;
	char		**argv;
	int			argc;

	int			end[2];

	int			export;

	int			status;
}				t_minishell;

typedef struct s_global
{
	int			g_status;
	t_m_free	*m_free;
}				t_global;

t_global	g_global;

char		**ft_pathfinder(t_env **head);
void		ft_parse_env(t_env **head, char **envp);
char		*ft_expand_var(t_env **head, char *cmds, bool dq, bool sq);
t_env		*add_node(t_env *head, char *key, char *value, bool equal);
int			ft_list_size(t_env *head);
char		*ft_get_env(t_env **head_env, char *key);
void		ft_set_env(t_env **head, char *key, char *value, bool equal);

char		*ft_strjoin_char(char const *s1, char c);
char		*ft_check_key(t_env *head, char *key);
char		**ft_strs_join(char **strs1, char **strs2);
char		**ft_remove_quotes(char **strs);
void		ft_print_without_backslash(char *string);

t_cmdlist	*ft_cmdlist(char *cmd_line, t_minishell *data);
void		ft_print_cmdlist(t_cmdlist *cmds);

void		pipex(t_minishell *data);

int			ft_error(char **parsed_line, int i);

char		*ft_remove_quote_in_string(char *string, bool quote, bool dquote);

/* Signals */

void		unplug_signals(void);

/* Built-ins */

int			ft_built_in_cd(t_env **head, char **full_cmd);
void		ft_built_in_exit(t_minishell *data, int in_pipe);
void		ft_built_in_env(char **envp);
void		ft_built_in_pwd(void);
void		ft_built_in_export(t_env **head, char **full_cmd);
void		ft_built_in_unset(t_env **head, char **key_list);

#endif
