/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:59 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/03 20:45:09 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// hi"hi"' '"hi"->hihi hi
// echo '"$?"'$?" ". -> "$?"0 .
int	check_unclosed_quotes(char *s, t_token **head, int n)
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

char	*remove_quo(char *s)
{
	int		i, k;
	char	c;
	char	*new;

	i = 0;
	k = 0;
	while (s[i++])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			while (s[i] && s[i] != c)
				i++;
			k = k + 2;	
		}
	}
	new = (char *)malloc(ft_strlen(s) - k + 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				new[k++] = s[i++];
			i++;	
		}
		new[k++] = s[i++];
	}
	new[k + 1] = 0;//?should i
	return (new);
}