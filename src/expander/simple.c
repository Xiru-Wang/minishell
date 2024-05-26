/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:18:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 10:05:14 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// in this function: vars expand without caring quotes
char	*expand_simple(char *s, t_data *data)
{
	int		i;
	char	*dst;
	char	*value;
	char	*temp;

	i = 0;
	dst = ft_calloc(1, sizeof(char));
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
			value = handle_dollar(s + i, &i, data->env, data);
		else
		{
			value = char_to_str(s[i]);
			i++;
		}
		temp = ft_strjoin(dst, value);
		free(dst);
		dst = temp;
		free(value);
	}
	return (dst);
}

//should check len within double quotes(include quotes)
int	len_within_quo(char *s, char c)
{
	int	len;

	len = 0;
	if (*s == c)
	{
		s++;
		while (*s && *s != c)
		{
			len++;
			s++;
		}
	}
	return (len + 2);
}
