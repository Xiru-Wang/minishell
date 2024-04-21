/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/21 19:33:22 by jschroed         ###   ########.fr       */
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

enum	s_type ft_type(char c)
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

int split_line(char *s, t_token **token_list, t_data *data)
{
	int i, res_token;
	enum s_type type;
	int res_quo;

	i = 0;
	while (s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (!s[i])
			break;
		type = ft_type(s[i]);
		if (type == WORD)
		{
			res_quo = check_unclosed_quotes((s + i), token_list, i);
			if (res_quo == -1)
				free_exit("unclosed quote", data, EXIT_FAILURE);
			i += res_quo;
		}
		else if (type == REDIR_IN && s[i + 1] && ft_type(s[i + 1]) == REDIR_IN)
		{
			res_token = add_token_list(ft_substr(s, i, 2), HEREDOC, token_list, i);
			if (res_token == 0)
				return (0);
			i += 2;
		}
		else if (type == REDIR_OUT && s[i + 1] && ft_type(s[i + 1]) == REDIR_OUT)
		{
			res_token = add_token_list(ft_substr(s, i, 2), APPEND, token_list, i);
			if (res_token == 0)
				return (0);
			i += 2;
		}
		else
		{
			res_token = add_token_list(ft_substr(s, i, 1), type, token_list, i);
			if (res_token == 0)
				return (0);
			i++;
		}
	}
	return (1);
}
