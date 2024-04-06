/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:49:23 by jschroed          #+#    #+#             */
/*   Updated: 2024/04/07 00:48:31 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libs/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>

#define MAX_FILES 10 //OPEN_MAX

extern int	g_exit_code;

typedef enum s_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	_SPACE,
	QUO,
}	t_type;

typedef enum s_builtin
{
	CD = 1,
	ECHO,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
}	t_builtin;

typedef struct s_token
{
	char			*value;
	t_type			type;
	int				i;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_io
{
	char			*filename;
	t_type			type;
	struct s_io		*next;
}	t_io;


typedef struct s_data t_data;
typedef struct s_cmd
{
	char			**s;
	t_builtin		is_builtin;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	struct s_io		*io_list;
	int				infd[MAX_FILES];
	int				outfd[MAX_FILES];
	int				last_fdin;
	int				last_fdout;
	int				id;
	char			*delimiter;
	char			*hdfile;
	t_data			*data;
} t_cmd;

typedef struct s_data
{
	char		*line;
	t_token		*token_list;
	char		*var_name;
	char		**env;
	t_cmd		*cmd_list;
	pid_t		*pid;
	int			cmd_num;
}	t_data;

// utils
int		is_space(char c);
int		if_all_space(char *s);
void	free_double_ptr(char **ptr);
void	free_cmd_list(t_cmd **cmd);
void	free_exit(char *s, t_data *data, int code);

//token_util
int		add_token_list(char *s, int type, t_token **head, int n);
void	del_token(t_token **head, t_token *node);
void	print_token_list(t_token *token_list);//debug
void	free_token_list(t_token **list);

// tokens
void	init_data(t_data *data, char **env);
int		split_line(char *s, t_token **token_list, t_data *data);

//check_quote
int		check_unclosed_quotes(char *s, t_token **head, int n);

//remove_quo
char	*remove_quo(char *s);

//expander
char	*expander(char *s, int len, t_data *data);
char	*find_var(char *var, int len, char **env);

//expand dollar in quote mixs
char	*handle_dollar_quo(char *s, t_data *data, enum s_type type);
char *replace_vars_complex(char *s, t_data *data);
void combine_strings(char *dst, char *s, char *value, int *i, int k);
char *expand_dollar(char *s, int *len, t_data *data);

//expander_utils
int check_valid_dollar(char *s);
int char_is_valid(char c);
int check_valid_dollar_limit(char *s, int max);

//heredoc_expander
char	*replace_vars_simple(char *s, t_data *data);

// cmd_utils
t_cmd	*init_cmd(t_data *data);
void	append_cmd(t_cmd **head, t_cmd *new);
int		count_pipe(t_token *list);
int		count_args(t_token *list);
void	print_cmd_list(t_cmd *cmd);//debug
								   //
//generate_cmd
t_cmd	*generate_cmds(t_token **token, t_data *data);

//io_utils
int		add_io_list(t_io **head);
void	free_io_list(t_io **list);

//io_redir
void	get_redir_fd_array(t_cmd *cmd);
void	redirect_fds(t_cmd *cmd, int *end);

//heredoc
int		check_hd(t_cmd *cmd);

// call_cmd
int		call_cmd(t_data *data, t_cmd *cmd);
char	*find_path(char *s, char **env);

//executor
int		executor(t_cmd *cmd, t_data *data);

//check_dollar_quo
char	*replace_vars_complex(char *s, t_data *data);

// builtin
enum s_builtin	ft_bubiltin(char *s);
void			call_builtin(t_cmd *cmd);
void			call_echo(t_cmd *cmd);
int		call_env(t_cmd *cmd);

#endif
