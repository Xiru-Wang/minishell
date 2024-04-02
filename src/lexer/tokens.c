/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/02 19:52:34 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_data(t_data *data, char **env)
{
	data->line = NULL;
	data->token_list = NULL;
	data->var_name = NULL;
	data->env = env;
	data->cmd_list = NULL;
	data->cmd_num = 0;

	//init_signals()
}

static enum	s_type ft_type(char c)
{
	if (c == '|')
		return (PIPE);
	else if (c == '<')
		return (REDIR_IN);
	else if (c == '>')
		return (REDIR_OUT);
	else if (is_space(c))
		return (_SPACE);
	return (WORD);
}

int	split_line(char *s, t_token **token_list, t_data *data)
{
	int			i, n, res;
	enum s_type	type;
	int			result;

	i = 0;
	n = 0;
	res = -1;
	while (s[i])
	{
		while (is_space(s[i]))
			i++;
		type = ft_type(s[i]);
		if (type == WORD)//can be s long string/cmd_name/or args
		{
			result = check_quotes((s + i), token_list, n);
			if (result == -1)
				free_exit("unclosed quote", data, EXIT_FAILURE);
			i += result;
		}
		else if (type == REDIR_IN && s[i + 1] && ft_type(s[i + 1]) == REDIR_IN)
		{
			res = add_token_list(ft_substr(s, i, 2), HEREDOC, token_list, n);
			i = i + 2;
		}
		else if ( type == REDIR_OUT && s[i + 1] && ft_type(s[i + 1]) == REDIR_OUT)
		{
			res = add_token_list(ft_substr(s, i, 2), APPEND, token_list, n);
			i = i + 2;
		}
		else
		{
			res = add_token_list(ft_substr(s, i, 1), type, token_list, n);
			i++;
		}
		if (res == 0)//add_list failed
			return (0);
		n++;
	}
	return (1);
}
