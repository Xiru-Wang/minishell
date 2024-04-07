/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comlex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/07 19:43:41 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*replace_vars_complex(char *s, char **env);
static char *handle_single_quote(char *s, int *i);
static char *handle_double_quote(char *s, int *i, char **env);
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

/* Expected: bla$USERwawUSERNAMEHOSTNAMEover,
 * Got:		 bla'$TESTUSER'waw"$TESTUSER""$TESTHOST"over */
static char	*replace_vars_complex(char *s, char **env)
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
        else if (s[i] == '\"')
            value = handle_double_quote(s, &i, env);
        else if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
            value = handle_dollar(s, &i, env);
        else
        {
            value = char_to_str(s[i]);
            i++;
        }
        temp = dst;
        dst = ft_strjoin(temp, value);
        free(temp);
        free(value);
	}
	return (dst);
}

static char *handle_single_quote(char *s, int *i)
{
	int k;
	char *value;

	k = len_within_quo(s + *i, '\'');
	value = ft_substr(s, *i, k);
	*i += k;
	return (value);
}

static char *handle_double_quote(char *s, int *i, char **env)
{
	int		k;
	char	*value, *temp;

	k = len_within_quo(s + *i, '\"');
	value = NULL;
	if (check_valid_dollar_limit(s + *i, k))
	{
        temp = ft_substr(s, *i, k);
        value = expand_simple(temp, env);
        free(temp);
    }
    else
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
	*i += k;
	return (value);
}

/*
static char	*replace_vars_complex(char *s, char **env)
{
	int		i, k;
	char	*dst, *value, *temp;

	dst = ft_calloc(sizeof(char), 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
		{
			k = len_within_quo(s + i, '\'');
			ft_strlcat(dst, s + i, ft_strlen(dst) + k + 1);
			i = i + k;
		}
		else if (s[i] == '\"')
		{
			k = len_within_quo(s + i, '\"');
			if (check_valid_dollar_limit(s + i, k))
			{
				temp = ft_substr(s, i, k);
				value = expand_simple(temp, env);
				ft_strlcat(dst, value, ft_strlen(dst) + ft_strlen(value) + 1);
				free(value);
				free(temp);
			}
			else
				ft_strlcat(dst, s + i, ft_strlen(dst) + k + 1);
			i = i + k;
		}
		else if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
		{
			k = 0;
			value = expand_dollar(s + i, &k, env);
			ft_strlcat(dst, value, ft_strlen(dst) + ft_strlen(value) + 1);
			free(value);
			i = i + k;
		}
		value = char_to_str(s[i]);
		ft_strlcat(dst, value, ft_strlen(dst) + 2);
		free(value);
		i++;
	}
	return (dst);
}
*/
