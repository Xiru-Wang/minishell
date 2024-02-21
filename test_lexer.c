/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/02/21 16:37:16 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	is_special(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

//think about << >> and quotes
static int	count_num_token(char *s)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && ft_is_space(*s))
			s++;
		if (!ft_is_space(*s) && !is_special(*s))//is word
		{
			count++;
			while (*s && !ft_is_space(*s) && !is_special(*s))//skip word
				s++;
		}
		else if (is_special(*s))//is sepcial
		{
			count++;
			s++;
		}
	}
	return (count);
}

char	**split_line(char *line)
{
	char *new;
	char **tokens;
	int start, num;

	//include \n??🤔
	new = ft_strtrim(line, " \t\n\v\f\r"); // Remove empty char at start and end
	printf("new: %s\n", new);
	num = count_num_token(new) + 1; //+1 for NULL terminator
	tokens = (char **)malloc(sizeof(char *) * num);
	if (!tokens)
	{
		free(new);
		return (NULL);
	}
	int i = 0;
	int n = 0;
	while (new[i])
	{
		while (ft_is_space(new[i]))// Skip spaces to find the start of next token
			i++;
		if (!is_special(new[i]))//not | < > << >>
		{
			start = i;
			//i++;
			while (new[i] && !ft_is_space(new[i]) && !is_special(new[i])) // skip rest of the word
				i++;
			tokens[n] = ft_substr(new, start, i - start);
			printf("token[%d]: %s, len: %d\n", n, tokens[n], i - start);//debug
			n++;
		}
		else if (is_special(new[i]))
		{
			tokens[n] = ft_substr(new, i, 1);//Handle special char as a separate token
			printf("token[%d]: %s, len: %d\n", n, tokens[n], 1);//debug
			n++;
			i++;
		}
	}
	free(new);
	tokens[num] = NULL;//terminate 1st dimension
	return (tokens);
}
