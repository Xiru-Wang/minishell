/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/23 19:33:50 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_single_quote(char *s, int *i);
static char	*handle_double_quote(char *s, int *i, t_data *data);

static int	check_quo(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_complex(char *s, t_data *data)
{
	char	*temp;
	char	*new;
	int		quo;

	temp = NULL;
	quo = check_quo(s);
	if (check_valid_dollar(s) == 0 && quo == 0)
		return (ft_strdup(s));
	else if (check_valid_dollar(s) == 0)
		return (remove_quo(s));
	else
	{
		temp = replace_vars_complex(s, data);
		if (quo == 1)
		{
			new = remove_quo(temp);
			free (temp);
			return (new);
		}
		return (temp);
	}
}

// if the string mixed normal chars and quotes
//eg. hihi$USER"'$USER'" --->hihixiwang'xiwang'
//eg.echo "$?'$?'$USER"  --->0'0'xiwang

char	*replace_vars_complex(char *s, t_data *data)

{
	int		i;
	char	*dst;
	char	*value;
	char	*temp;

	i = 0;
	dst = (char *)ft_calloc(1, sizeof(char));
	while (s[i])
	{
		if (s[i] == '\'')
			value = handle_single_quote(s, &i);
		else if (s[i] == '\"')
			value = handle_double_quote(s, &i, data);
		else if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
			value = handle_dollar(s, &i, data->env, data);
		else
		{
			value = char_to_str(s[i]);
			i++;
		}
		// if (value != NULL)
		// {
			temp = dst;
			dst = ft_strjoin(temp, value);
			free(temp);
			free(value);
		//}
	}
	return (dst);
}

static char	*handle_single_quote(char *s, int *i)
{
	int		k;
	char	*value;

	k = len_within_quo(s + *i, '\'');
	value = ft_substr(s, *i, k);
	*i += k;
	return (value);
}

static char	*handle_double_quote(char *s, int *i, t_data *data)
{
	int		k;
	char	*value;
	char	*temp;

	k = len_within_quo(s + *i, '\"');
	temp = ft_substr(s, *i, k);
	value = expand_simple(temp, data);
	free(temp);
	*i += k;
	return (value);
}
