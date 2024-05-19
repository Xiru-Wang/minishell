/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:59 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/19 19:59:35 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_next_quo(char *s, int *i, int *quote_sign)
{
	char	c;

	c = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != c)
		(*i)++;
	if (s[*i] && s[*i] == c)
	{
		(*i)++;
		*quote_sign = 1;
	}
	else
		return (-1);
	return (0);
}

int check_unclosed_quotes(char *s, t_token **head)
{
	int		i;
	int		quote_sign;

	i = 0;
	quote_sign = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (find_next_quo(s, &i, &quote_sign) == -1)
				return (-1);
		}
		else if (ft_type(s[i]) != WORD)
		{
			if (quote_sign == 1)
				add_token_list(ft_substr(s, 0, i), QUO, head);
			else if (i > 0)
				add_token_list(ft_substr(s, 0, i), WORD, head);
			return (i);
		}
		else
			i++;
	}
	if (quote_sign == 1)
		add_token_list(ft_substr(s, 0, i), QUO, head);
	else if (i > 0)
		add_token_list(ft_substr(s, 0, i), WORD, head);
	return (i);
}

// int check_unclosed_quotes(char *s, t_token **head)
// {
// 	int		i;
// 	char	c;
// 	int		quote_sign;

// 	i = 0;
// 	quote_sign = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '\'' || s[i] == '\"')
// 		{
// 			c = s[i];
// 			i++;
// 			while (s[i] && s[i] != c)
// 				i++;
// 			if (s[i] == c)
// 			{
// 				i++;
// 				quote_sign = 1;
// 			}
// 			else
// 				return (-1);
// 		}
// 		else if (ft_type(s[i]) != WORD)
// 		{
// 			if (quote_sign == 1)
// 				add_token_list(ft_substr(s, 0, i), QUO, head);
// 			else if (i > 0)
// 				add_token_list(ft_substr(s, 0, i), WORD, head);
// 			return (i);
// 		}
// 		else
// 			i++;
// 	}
// 	if (quote_sign == 1)
// 		add_token_list(ft_substr(s, 0, i), QUO, head);
// 	else if (i > 0)
// 		add_token_list(ft_substr(s, 0, i), WORD, head);
// 	return (i);
// }
