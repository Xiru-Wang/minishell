/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/02/22 16:19:55 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

//Once a line has been inputted it checks for any unclosed quotes.
//If it doesn't find any it sends the line to the lexer.

int	ft_type(char c)
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

//think about << >> and quotes
// if i use linkedlist no need to consider
// static int	count_num_token(char *s)
// {
// 	int	count;

// 	count = 0;
// 	while (*s)
// 	{
// 		while (*s && ft_is_space(*s))
// 			s++;
// 		if (!ft_is_space(*s) && !is_special(*s))//is word
// 		{
// 			count++;
// 			while (*s && !ft_is_space(*s) && !is_special(*s))//skip word
// 				s++;
// 		}
// 		else if (is_special(*s))//is sepcial
// 		{
// 			count++;
// 			s++;
// 		}
// 	}
// 	return (count);
// }

void	split_line(char *s)
{
	int start;
	char *temp;

	//new = ft_strtrim(line, " \t\n\v\f\r"); // Remove empty char at start and end
	int i = 0;
	int n = 0;
	while (s[i])
	{
		while (ft_is_space(s[i]))// Skip spaces to find the start of next token
			i++;
		int type = ft_type(s[i]);
		if (type == WORD || type == ARG)
		{
			start = i;
			while (s[i] && !ft_is_space(s[i]) && (type == WORD || type == ARG)) // skip rest of the word
				i++;
			temp = ft_substr(s, start, i - start);
			add_node(temp, type, n);
		}
		else if (type == REDIR_IN && ft_type(s[i + 1]) == REDIR_IN)
		{
			add_node("<<", HEREDOC, n);
			i = i + 2;
		}
		else if ( type == REDIR_OUT && ft_type(s[i + 1]) == REDIR_OUT)
		{
			add_node(">>", APPEND, n);
			i = i + 2;
		}
		else
		{
			temp = ft_substr(s, i, 1);//需要给node里的string malloc吗？
			add_node(s, type, n);
			i++;
		}
		n++;
	}
}

int main()
{
		char *line;
		char **tokens;
		line = "  hello 42 ? |>< cat|  	";

		printf("before split_line\n");
		tokens = split_line(line);
		int i = 0;
		while (tokens[i])
		{
			printf("token[%d]:%s\n", i, tokens[i]);
			i++;
		}
		free_double_ptr(tokens);
		printf("exit\n");
		return (0);
}
