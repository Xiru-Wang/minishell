/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:19:05 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 19:34:51 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_valid_dollar(char *s)
{
	while (*s)
	{
		if (*s == '$' && *(s + 1))
		{
			s++;
			if (char_is_valid(*s))
				return (1);
		}
		s++;
	}
	return (0);
}

// if "'$USER'"$? -> 'xiwang'0
int	check_valid_dollar_limit(char *s, int max)
{
	int	i;

	i = 0;
	printf("%d\n", max);
	while (s[i] && i < max)
	{
		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
			return (1);
		i++;
	}
	return (0);
}
