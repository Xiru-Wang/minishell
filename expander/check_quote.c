/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:59 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/02 19:42:33 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// hi"hi"' '"hi"->hihi hi
// echo '"$?"'$?" ". -> "$?"0 .
int	check_quotes(char *s, t_token **head, int n)
{
	int		i;
	char	c;
	int		quote_sign;

	i = 0;
	quote_sign = 0;
	while (s[i++])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			while (s[i])
				if (s[i] != c)
					i++;
			if (s[i] == c)//found closing quote
			{
				i++;
				quote_sign = 1;
			}
			else
				return (-1);//unclosed quote
		}
		if (s[i] >= PIPE && s[i] <= _SPACE)//meet token
		{
			if (quote_sign == 1)
				add_token_list(ft_substr(s, 0, i), QUO, head, n);//quote can mix
			else
				add_token_list(ft_substr(s, 0, i), WORD, head, n);//quote can mix
			return (i);
		}
	}
	return (0);
}

char	*remove_quo_expand(char *s, t_data *data)
{
	int		i, k, j;
	char	*new, *temp, *s2;//expaned string

	i = 0;
	k = 0;
	new = (char *)malloc(1024); // assume this size
	while (s[i])
	{
		if (s[i] == '\'')
		{
			i++;//skip left single quote
			while (s[i] && s[i] != '\'')
				new[k++] = s[i++];//copy content between S_QUO
			i++; // skip right single quote
		}
		else if (s[i] == '\"')
		{
			i++;//skip left double quote
			j = i;
			while (s[i] && s[i] != '\"')
				i++;
			temp = ft_substr(s, j, i - j);
			s2 = handle_dollar(temp, data);
			free(temp);
			i++;
			j = 0;
			while (s2[j])
				new[k++] = s2[j++];
			free(s2);
		}
		else
			new[k++] = s[i++];
	}
	new[k] = 0;
	return (new);
}
