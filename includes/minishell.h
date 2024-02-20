/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:49:23 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/20 21:14:08 by xiwang           ###   ########.fr       */
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

# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7

# define STDIN 0
# define STDOUT 1
# define STDERR 2

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_session
{
	char		*line;
	char		**tokens;
	char		**env;
	char		*pwd;
	char		*old_pwd;
	int			num_tokens;
	t_token *token;
	// t_command	*cmd;
	// pid_t		pid;
}	t_session;


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
char	**split_line(const char *line);
#endif
