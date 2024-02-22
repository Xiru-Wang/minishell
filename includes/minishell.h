/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:49:23 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/22 19:32:30 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

// # define CMD 1
// # define ARG 2
// # define TRUNC 3
// # define APPEND 4
// # define INPUT 5
// # define PIPE 6
// # define END 7

# define STDIN 0
# define STDOUT 1
# define STDERR 2

typedef enum s_type
{
	WORD,
	ARG,//eg. -e -ls
	PIPE,
	REDIR_IN,//<
	REDIR_OUT,//>
	HEREDOC,// <<
	APPEND,//>>
}	t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	int				i;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	char		*line;
	t_token		**token_list;
	char		**env;
	char		*pwd;
	char		*old_pwd;
	int			num_tokens;
	t_token		*token;
	// t_command	*cmd;
	// pid_t		pid;
}	t_data;


// termination
// void	exit_shell(t_session *session, int exit_number);
// void	free_session(t_session *session, bool clear_history);
// void	free_array_str(char **array);
// void	free_pointer(void *ptr);
//utils
int		ft_is_space(char c);
int		if_all_space(char *s);
void	free_double_ptr(char **ptr);
//lexer
void	init_data(t_data *data, char **env);
void	split_line(char *s, t_data *data);
//token_list
t_token	*create_token(char *s, t_type type, int n);
void	token_add_back(t_token **head, t_token *new);
int		add_list(char *str, t_type type, t_token **head, int n);
void	print_list(t_token *token_list);
void	free_node(t_token **list);

#endif
