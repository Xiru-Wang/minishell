/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/02 19:01:01 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *handle_single_quote(char *s, int *i);
static char *handle_dollar(char *s, int *i, char **env);

char	*expand_complex(char *s, enum s_type type, t_data *data) // should not free s here?
{
	char	*temp;
	char	*new;

	if (check_valid_dollar(s) == 0 && type == WORD) // if no valid dollar: do nothing
		return (ft_strdup(s));
	else if (check_valid_dollar(s) == 0 && type == QUO)
		return(remove_quo(s));
	else
	{
		temp = replace_vars_complex(s, data->env);//single_quo && dollar
		if (type == QUO)
		{
			new = remove_quo(temp);
			free(temp);
			return(new);
		}
		return (temp);
	}
}

char	*replace_vars_complex(char *s, char **env)
{
	int i;
	char *dst, *value, *temp;

	i = 0;
	dst = ft_calloc(sizeof(char), 1);
	value = NULL;
	temp = NULL;
	while (s[i])
	{
		if (s[i] == '\'')
			value = handle_single_quote(s, &i);
		else if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
			value = handle_dollar(s, &i, env);
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

static char *handle_single_quote(char *s, int *i)
{
	int		k;
	char	*value;

	k = len_within_quo(s + *i, '\'');
	value = ft_substr(s, *i, k);
	*i += k;
	return (value);
}

static char *handle_dollar(char *s, int *i, char **env)
{
	int		k;
	char	*value;

	k = 0;
	value = expand_dollar(s + *i, &k, env);
	if (value == NULL)
		value = ft_strdup("");
	*i += k;
	return (value);
}