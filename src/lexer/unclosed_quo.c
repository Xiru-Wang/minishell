/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:59 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/25 19:14:20 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_next_quo(char *s, int *i)
{
	char	c;
	int		sign;

	sign = 0;
	c = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != c)
		(*i)++;
	if (s[*i] && s[*i] == c)
	{
		(*i)++;
		sign = 1;
	}
	else
		return (-1);
	return (sign);
}

//handle complex situation, eg: echo "hi"hi"$?" | grep hi
//second token value = "hi"hi"$?"
int	check_unclosed_quotes(char *s, t_token **head, t_data *data)
{
	int	sign;
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			sign = find_next_quo(s, &i);
			if (sign == -1)
				return (-1);
		}
		else if (tk_type(s[i]) != WORD)
		{
			add_token_list(ft_substr(s, 0, i), STR, head, data);
			return (i);
		}
		else
			i++;
	}
	add_token_list(ft_substr(s, 0, i), STR, head, data);
	return (i);
}
