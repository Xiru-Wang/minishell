/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/03/08 11:05:32 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

//Once a line has been inputted it checks for any unclosed quotes.
//If it doesn't find any it sends the line to the lexer.

void	init_data(t_data *data, char **env)
{
	data->token_list = NULL;
	data->env = env;
	data->cmd_list = NULL;
	data->cmd_num = 0;
	data->in = NULL;
	data->out = NULL;
	data->append = NULL;
	data->line = NULL;
	data->var_name = NULL;

	//init_signals()
}

static enum	s_type ft_type(char c)
{
	if (c == '-')
		return (ARG);
	else if (c == '|')
		return (PIPE);
	else if (c == '<')
		return (REDIR_IN);
	else if (c == '>')
		return (REDIR_OUT);
	else if (c == '\'')
		return (S_QUO);
	else if (c == '\"')
		return (D_QUO);
	return (WORD);
}

/*
它提供了更强的类型安全和表达性。使用枚举类型的变量意味着这个变量只能接受那个枚举定义的值，
这有助于避免错误地将不相关的整数值赋给变量，也使得你的代码意图更加明确。
*/
int	split_line(char *s, t_token **token_list, t_data *data)
{
	int	i, start, n, res;
	enum s_type	type;
	int		result;

	i = 0;
	n = 0;
	res = 1;
	while (s[i])
	{
		type = ft_type(s[i]);
		if (type == WORD || type == ARG)
		{
			start = i;
			while (s[i] && !is_space(s[i]) && (ft_type(s[i]) == WORD || ft_type(s[i]) == ARG)) // skip rest of a word
				i++;
			res = add_list(ft_substr(s, start, i - start), type, token_list, n);
		}
		else if (type == REDIR_IN && s[i + 1] && ft_type(s[i + 1]) == REDIR_IN)
		{
			res = add_list(ft_substr(s, i, 2), HEREDOC, token_list, n);
			i = i + 2;
		}
		else if ( type == REDIR_OUT && s[i + 1] && ft_type(s[i + 1]) == REDIR_OUT)
		{
			res = add_list(ft_substr(s, i, 2), APPEND, token_list, n);
			i = i + 2;
		}
		else if (type == S_QUO || type == D_QUO)
		{
			result = handle_quotes((s + i), type, token_list, n);
			if (result == -1)
				free_exit("unclosed quote", data, EXIT_FAILURE);
			i += result;
		}
		else
		{
			res = add_list(ft_substr(s, i, 1), type, token_list, n);
			i++;
		}
		if (res == 0)
			return (0);
		n++;
		while (is_space(s[i]))
			i++;
	}
	return (1);
}
