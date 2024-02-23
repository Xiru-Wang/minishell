/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/02/23 19:47:05 by xiruwang         ###   ########.fr       */
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
	return (WORD);
}

int	split_line(char *s, t_token **token_list)
{
	int	i, start, n;
	int	type;
	char *temp;

	i = 0;
	n = 0;
	while (s[i])
	{
		printf("in the loop\n");
		type = ft_type(s[i]);
		if (type == WORD || type == ARG)
		{
			printf("%c\n", s[i]);
			start = i;
			while (s[i] && !ft_is_space(s[i]) && (type == WORD || type == ARG)) // skip rest of the word
				i++;
			printf("here***\n");
			printf("i = %d, start = %d\n", i, start);
			temp = ft_substr(s, start, i - start);
			printf("%s\n", temp);
			printf("the problem is after this line\n");
			if (add_list(temp, type, token_list, n) == 0)
				return (0);
		}
		else if (type == REDIR_IN && ft_type(s[i + 1]) == REDIR_IN)
		{
			temp = ft_substr(s, i, 2);
			if (!add_list(temp, HEREDOC, token_list, n))
				return (0);
			i = i + 2;
		}
		else if ( type == REDIR_OUT && ft_type(s[i + 1]) == REDIR_OUT)
		{
			temp = ft_substr(s, i, 2);
			if (!add_list(temp, APPEND, token_list, n))
				return (0);
			i = i + 2;
		}
		else if (type != WORD && type != ARG)
		{
			temp = ft_substr(s, type, 1);
			if (add_list(temp, type, token_list, n))
				return (0);
			i++;
		}
		n++;
		while (ft_is_space(s[i]))// Skip spaces to find the start of next token
			i++;
	}
	free(temp);
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
