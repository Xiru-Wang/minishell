/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/02/27 12:39:26 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

//Once a line has been inputted it checks for any unclosed quotes.
//If it doesn't find any it sends the line to the lexer.

void	init_data(t_data *data, char **env)
{
	data->token_list = NULL;
	data->env = env;

	//init_signals();
	//return (1);
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

int	split_line(char *s, t_token **token_list, t_data *data)
{
	int	i, start, n;
	int	type, result;

	i = 0;
	n = 0;
	while (s[i])
	{
		type = ft_type(s[i]);
		if (type == WORD || type == ARG)
		{
			start = i;
			while (s[i] && !ft_is_space(s[i]) && (ft_type(s[i]) == WORD || ft_type(s[i]) == ARG)) // skip rest of a word
				i++;
			if (add_list(ft_substr(s, start, i - start), type, token_list, n) == 0)
				return (0);
		}
		else if (type == REDIR_IN && s[i + 1] && ft_type(s[i + 1]) == REDIR_IN)
		{

			if (!add_list(ft_substr(s, i, 2), HEREDOC, token_list, n))
				return (0);
			i = i + 2;
		}
		else if ( type == REDIR_OUT && s[i + 1] && ft_type(s[i + 1]) == REDIR_OUT)
		{
			if (!add_list(ft_substr(s, i, 2), APPEND, token_list, n))
				return (0);
			i = i + 2;
		}
		else if (type == S_QUO || type == D_QUO)
		{
			result = handle_quotes((s + i), type, token_list, n);
			if (result == -1)
			{
				printf("unclosed quote\n");
				free_exit(data);
				exit(EXIT_FAILURE);
			}
			i += result;
		}
		else
		{
			if (!add_list(ft_substr(s, i, 1), type, token_list, n))
				return (0);
			i++;
		}
		n++;
		while (ft_is_space(s[i])) // Skip spaces to find the start of next token
			i++;
	}
	return (1);
}

// int main()
// {
// 		char *line;
// 		char **tokens;
// 		line = "  hello 42 ? |>< cat|  	";

// 		printf("before split_line\n");
// 		tokens = split_line(line);
// 		int i = 0;
// 		while (tokens[i])
// 		{
// 			printf("token[%d]:%s\n", i, tokens[i]);
// 			i++;
// 		}
// 		free_double_ptr(tokens);
// 		printf("exit\n");
// 		return (0);
// }
