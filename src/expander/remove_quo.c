/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:18:57 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/23 15:07:34 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_valid_quotes(char *s)
{
	int		i;
	int		count;
	char	c;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				i++;
			if (s[i] && s[i] == c)
			{
				i++;
				count += 2;
			}
		}
		else
			i++;
	}
	return (count);
}

char	*remove_quo(char *s)
{
	int		i;
	int		k;
	char	*new;
	char	c;

	i = 0;
	k = 0;
	new = (char *)malloc(ft_strlen(s) - count_valid_quotes(s) + 1);
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				new[k++] = s[i++];
			if (s[i] == c)
				i++;
		}
		else
			new[k++] = s[i++];
	}
	new[k] = '\0';
	return (new);
}

char	*remove_quo_simple(char *s)
{
	int	k;

	k = ft_strlen(s) - 1;
	if ((s[0] == '\'' && s[k] == s[0]) || (s[0] == '\"' && s[k] == s[0]))
		return (ft_substr(s, 1, ft_strlen(s) - 2));
	else
		return (ft_strdup(s));
}
