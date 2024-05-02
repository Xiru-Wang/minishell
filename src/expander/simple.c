/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:18:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/02 20:10:26 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_var_len(char *var);

// in this function: vars expand without careing quotes
char	*expand_simple(char *s, char **env)
{
	int		i;
	int		k;
	char	*dst;
	char	*value;

	dst = ft_calloc(sizeof(char), 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
		{
			k = 0;
			value = expand_dollar(s + i, &k, env);
			if (value)
			{
				ft_strlcat(dst, value, ft_strlen(dst) + ft_strlen(value) + 1);
				free(value);
			}
			i = i + k;
		}
		value = char_to_str(s[i]);
		ft_strlcat(dst, value, ft_strlen(dst) + 2);
		free(value);
		i++;
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

char	*expand_dollar(char *s, int *len, char **env)
{
	int		var_len;
	char	*var_name;
	char	*value;

	value = NULL;
	if (*s == '$')
	{
		s++;
		if (*s == '?')
		{
			value = ft_itoa(g_exit_code);
			*len = 2;
		}
		else if (ft_isdigit(*s))
		{
			*len = 2;
			return (NULL);
		}
		else
		{
			var_len = count_var_len(s);
			var_name = ft_substr(s, 0, var_len);
			value = find_env(var_name, env);
			free(var_name);
			*len = 1 + var_len;
		}
	}
	return (value);
}

static int	count_var_len(char *var)
{
	int	i;

	i = 0;
	while (ft_isalnum(var[i]) || var[i] == '_')
		i++;
	return (i);
}
