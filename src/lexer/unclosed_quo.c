/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:59 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/07 00:36:57 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
