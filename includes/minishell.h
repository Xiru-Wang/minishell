/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:49:23 by jschroed          #+#    #+#             */
/*   Updated: 2024/03/24 23:01:29 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef enum s_type
{
	WORD,
	ARG,//with dash eg.-ls
	PIPE,
	REDIR_IN,//<
	REDIR_OUT,//>
	HEREDOC,// << 对于同一个命令，后面的输入重定向会覆盖前面的
	APPEND,//>>
	S_QUO,
	D_QUO,
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

typedef struct s_cmd
{
	char			**s;
	t_builtin		is_builtin;
	struct s_cmd	*next;
	int				infd;
	int				outfd;
	int				id;
	t_type			o_type;
	t_type			in_type;
	char			*infile;
	char			*outfile;
	char			*delimiter;
	char			*hdfile;
	t_data			*data;
} t_cmd;

typedef struct s_data
{
	char		*line;
	t_token		*token_list;
	char		**env;
	int			fd_in;
	int			fd_out;
	char		*infile;
	char		*outfile;
	char		*append;
	t_cmd		*cmd_list;
	char		*var_name;
	int			cmd_num;
	// pid_t		pid;
}	t_data;

// utils
int		is_space(char c);
int		if_all_space(char *s);
void	free_double_ptr(char **ptr);
void	free_exit(char *s, t_data *data, int code);

//token_util
t_token *create_token(char *s, int type, int n);
void token_add_back(t_token **head, t_token *new);
int add_list(char *s, int type, t_token **head, int n);

//tokens: convert a string to a token list
void	init_data(t_data *data, char **env);
int		split_line(char *s, t_token **token_list, t_data *data);

//token_list.c
int		add_list(char *str, int type, t_token **head, int n);
void	print_list(t_token *token_list);//debug
void	free_token_list(t_token **list);

//quote.c
int		handle_quotes(char *s, int quote, t_token **head, int n);

//parser_utils
int		count_pipe(t_token *list);
int		count_args(t_token *list);
t_cmd	*creat_cmd(int size, t_data *data);
void	append_cmd(t_cmd **head, t_cmd *new);
void	del_token(t_token **head, t_token *node);
void	print_cmd_list(t_cmd *cmd);//debug

//parser: generat cmds from token list
void	extract_redir(t_token **head, t_data *data);
t_cmd	*generate_cmd(t_token **token, t_data *data);
void	fill_cmd(t_token **head, t_cmd *cmd, t_data *data);

//expansion
char	*handle_dollar(char *s, t_data *data);

//call_cmd
int	call_cmd(t_data *data, t_cmd *cmd);
//executor utils
void	assign_cmd_id(t_cmd *cmd);
//builtin
t_builtin	ft_bubiltin(char *s);
//heredoc
void	create_hd(t_cmd *cmd, t_data *data);

#endif
