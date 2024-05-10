/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/10 20:17:38 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_single_quote(char *s, int *i);
static char	*handle_double_quote(char *s, int *i, char **env, t_data *data);
static char	*handle_dollar(char *s, int *i, char **env, t_data *data);

char	*expand_complex(char *s, enum s_type type, t_data *data)
{
	char	*temp;
	char	*new;

	if (check_valid_dollar(s) == 0 && type == WORD)
		return (ft_strdup(s));
	else if (check_valid_dollar(s) == 0 && type == QUO)
		return (remove_quo(s));
	else
	{
		temp = replace_vars_complex(s, data->env, data);
		if (type == QUO)
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

char	*replace_vars_complex(char *s, char **env, t_data *data)

{
	int		i;
	char	*dst;
	char	*value;
	char	*temp;

	i = 0;
	dst = ft_calloc(sizeof(char), 1);
	value = NULL;
	temp = NULL;
	while (s[i])
	{
		if (s[i] == '\'')
			value = handle_single_quote(s, &i);
		else if (s[i] == '\"')
			value = handle_double_quote(s, &i, env, data);
		else if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
			value = handle_dollar(s, &i, env, data);
		else
		{
			value = char_to_str(s[i]);
			i++;
		}
		if (value != NULL)
		{
			temp = dst;
			dst = ft_strjoin(temp, value);
			free(temp);
			free(value);
		}
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

static char	*handle_double_quote(char *s, int *i, char **env, t_data *data)
{
	int		k;
	char	*value;
	char	*temp;

	k = len_within_quo(s + *i, '\"');
	temp = ft_substr(s, *i, k);
	value = expand_simple(temp, env, data);
	free(temp);
	*i += k;
	return (value);
}

static char	*handle_dollar(char *s, int *i, char **env, t_data *data)
{
	int		k;
	char	*value;

	k = 0;
	value = expand_dollar(s + *i, &k, env, data);
	if (value == NULL)
		value = ft_strdup("");
	*i += k;
	return (value);
}
