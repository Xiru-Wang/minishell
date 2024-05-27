/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:49:23 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/27 17:39:56 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "exit_codes.h"
# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libs/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <errno.h>

typedef enum s_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	_SPACE,
	STR,
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
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_io
{
	char			*filename;
	char			*hdfile;
	t_type			type;
	char			*eof;
	struct s_io		*next;
	int				eof_quo;
}	t_io;

typedef struct s_data	t_data;

typedef struct s_cmd
{
	char			**s;
	t_builtin		is_builtin;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	struct s_io		*io_list;
	int				infd;
	int				outfd;
	t_data			*data;
	int				stdin_backup;
	int				stdout_backup;
	int				err;
	int				empty_var;
}	t_cmd;

typedef struct s_data
{
	char		*line;
	t_token		*token_list;
	char		*var_name;
	char		**env;
	t_cmd		*cmd_list;
	pid_t		*pid;
	int			cmd_num;
	char		*pwd;
	char		*old_pwd;
	int			exit_code;
	int			in_heredoc;
}	t_data;

// needs to align with t_data def.
extern int				g_last_signal;

// utils
int				is_space(char c);
int				is_str_digit(char *str);
int				if_all_space(char *s);
void			free_double_ptr(char **ptr);
void			free_cmd_list(t_cmd **cmd);
void			free_exit(char *s, t_data *data, int code);
char			**ft_arrdup(char **arr);
void			print_error( \
					const char *prefix, const char *arg, const char *suffix);

// utils_free
void			free_data(t_data *data);

//token_util
int				add_token_list(char *s, int type, t_token **head, t_data *data);
void			del_token(t_token **head, t_token *node);
void			free_token_list(t_token **list);
enum s_type		tk_type(char c);

// tokens
void			init_data(t_data *data, char **env);
void			split_line(char *s, t_token **head, t_data *data);
//check_quote
int				check_unclosed_quotes(char *s, t_token **head, t_data *data);
//remove_quo
char			*remove_quo(char *s);
char			*remove_quo_simple(char *s);
// simple_expander
char			*expand_simple(char *s, t_data *data);
char			*expand_dollar(char *s, int *len, char **env, t_data *data);
int				len_within_quo(char *s, char c);
// complex_expander
char			*expand_complex(char *s, t_data *data);
char			*replace_vars_complex(char *s, t_data *data);
int				check_quo(char *s);
char			*append_value(char *dst, char *value);

//expander_utils
char			*char_to_str(char c);
int				check_valid_dollar(char *s);
int				char_is_valid(char c);
int				check_valid_dollar_limit(char *s, int max);
char			*find_env(char *s, char **env);

char			*handle_dollar(char *s, int *i, char **env, t_data *data);

// cmd_utils
t_cmd			*init_cmd(t_data *data);
void			append_cmd(t_cmd **head, t_cmd *new);
int				count_pipe(t_token *list);
int				count_args(t_token *list);
//sytax error
void			expand_arg(t_token *temp, t_token *next, t_cmd *cmd, int *i);
int				check_syntax(t_token *next, t_data *data);
int				pipe_syntax(t_token *token, t_data *data);
int				check_empty_token(t_token *token, t_data *data);

//generate_cmd
int				generate_cmds(t_token **token, t_cmd **cmd_list, t_data *data);

//io_utils
t_io			*init_io(t_cmd *cmd);
void			append_io(t_io **head, t_io *new);
void			free_io_list(t_io **list);
void			reset_stdio(t_cmd *cmd);
void			setup_stdio_backups(t_cmd *cmd);

//io_redir
void			get_fds(t_cmd *cmd);
void			redirect_io(t_cmd *cmd);

//heredoc
int				check_hd(t_cmd *cmd);

// call_cmd
int				call_cmd(t_data *data, t_cmd *cmd);
int				find_executable_and_execute(t_cmd *cmd, t_data *data);
char			*find_path(char *s, char **env);

// call_cd
char			*find_env_var(t_data *data, const char *var_name);
void			add_new_env_var(t_data *data, \
								const char *var_name, \
								const char *new_value, int i);
char			*handle_cd_oldpwd(t_data *data);
int				change_directory(t_data *data, char *path);
void			add_new_env_var(t_data *data, \
								const char *var_name, \
								const char *new_value, int i);
void			print_cd_error(char *path);
void			update_pwd_variables(t_data *data);
void			update_env_var(t_data *data, \
								const char *var_name, \
								const char *new_value);

//executor
int				executor(t_cmd *cmd, t_data *data);

// builtin
enum s_builtin	ft_builtin(char *s);
int				call_builtin(t_cmd *cmd);
int				call_echo(t_cmd *cmd);
int				call_env(t_cmd *cmd);
int				call_exit(t_cmd *cmd, t_data *data);
int				empty_str_exit(char *s);
int				print_error_exit(char *arg);
int				too_many_arguments_exit(char *arg);
int				call_pwd(t_cmd *cmd);
int				call_unset(t_cmd *cmd, t_data *data);
int				call_cd(t_data *data, t_cmd *cmd);
char			*resolve_cd_path(t_data *data, t_cmd *cmd);

// builtin export
int				call_export(t_cmd *cmd, t_data *data);
int				get_env_len(char **env);
char			**allocate_new_env(int len);
int				copy_env_vars(char **new_env, char **env, int len);
char			**add_str_to_env(char **new_env, char *str, int len);
int				is_valid_identifier(char c);
int				export_error(char *str);
void			delete_quotes(char *str, char quote);
char			**add_var(char **env, char *str);
int				handle_quotes_and_check(char *str, int pos);
int				add_new_var(t_data *data, char *str);
int				find_var_position(t_data *data, char *str, int pos);
int				update_or_add_var(t_data *data, char *str);
int				equal_sign(char *str);
char			*expand_tilde(char *arg);

// signals
void			signal_handler_hd(int signum);
int				readline_event_hook_hd(void);
void			init_signals_hd(void);
void			setup_signals_hd(void);
void			reset_signals_hd(void);
void			signal_handler(int signum);
void			init_signals(void);
void			backup_stdio(t_cmd *cmd);
void			init_signals_noint(void);

//DEBUG
void			print_cmd_list(t_cmd *cmd);
void			print_io_list(t_cmd *cmd);
void			print_token_list(t_token *token_list);

#endif
