/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:18:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/24 19:45:29 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*add_expand_value(char *s, char *dst, t_data *data, int *i)
{
	char	*value;
	char	*new;

	value = handle_dollar(s, i, data->env, data);
	new = ft_strjoin(dst, value);
	free(dst);
	free(value);
	return (new);
}

// in this function: vars expand without careing quotes
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
			dst = add_expand_value(s + i, dst, data, &i);
		else
		{
			value = char_to_str(s[i]);
			temp = ft_strjoin(dst, value);
			free(dst);
			dst = temp;
			free(value);
			i++;
		}
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
