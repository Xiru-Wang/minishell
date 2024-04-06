/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dollar_quo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/07 00:41:11 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_var_len(char *var);
static char	*expand_dollar(char *s, int *len, t_data *data);
static int	len_within_quo(char *s, char c);

char	*handle_dollar_quo(char *s, t_data *data, enum s_type type)//should not free s here?
{
	char	*temp;
	char	*new;

	if (check_valid_dollar(s) == 0 && type == WORD) // if no valid dollar: do nothing
		return (ft_strdup(s));
	else if (check_valid_dollar(s) == 0 && type == QUO)
		return(remove_quo(s));
	else
	{
		temp = replace_vars_complex(s, data);//single_quo && dollar
		if (type == QUO)
		{
			new = remove_quo(temp);
			free(temp);
			return(new);
		}
		return (temp);
	}
}

// this function should handle this case
// in: bla'$USER'waw"$USER""$?"over
// out: bla'$USER'wawUSERNAME0over
char *replace_vars_complex(char *s, t_data *data)
{
    int     i, var_len, flag, k;
    char    *dst, *value, *temp;

    flag = 0;
    dst = ft_calloc(1, 1);
    i = 0;
    while (s[i])
    {
        if (s[i] == '\'')
            i += len_within_quo(s + i, '\'');
		else if (s[i] == '\"')
		{
			k = len_within_quo(s + i, '\"');
			if (check_valid_dollar_limit(s + i, k))
			{
				flag = 1;
				temp = ft_substr(s, i, i + k);
				value = replace_vars_simple(temp, data);
				if (value)
					combine_strings(dst, s, value, &i, k);
				free(temp);
			}
			else
				i = i + k;
		}
		else if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
        {
            flag = 1;
            k = 0;
            value = expand_dollar(s + i, &k, data);
            if (value)
				combine_strings(dst, s, value, &i, k);
        }
		i++;
	}
	if (flag)
        return (dst);
    free(dst);
    return (ft_strdup(s));
}

void combine_strings(char *dst, char *s, char *value, int *i, int k)
{
	ft_strlcat(dst, s, ft_strlen(dst) + *i + 1);//before
	ft_strlcat(dst, value, ft_strlen(dst) + ft_strlen(value) + 1);//insert content
	*i = *i + k;
	ft_strlcat(dst, s + *i, ft_strlen(dst) + ft_strlen(s) - *i + 1);//after
	free(value);
}

//count chars from 1st s_quo to next s_quo
static int	len_within_quo(char *s, char c)
{
	int	len;

	len = 0;
	if (*s == c) // len_s_quo
	{
		len++;
		while (*s && *s != c)
			len++;
		if (*s == c)
			len++;
	}
	return (len);
}

static int count_var_len(char *var)
{
    int i;

    i = 0;
    while (ft_isalnum(var[i]) || var[i] == '_')
        i++;
    return (i);
}

char	*expand_dollar(char *s, int *len, t_data *data)
{
	int		var_len;
	char	*var_name;
	char	*value;

	value = NULL;
	if (*s == '$')
	{
		s++; // skip dollar
		if (ft_isdigit(*s)) // skip 1st digit
		{
			*len = 2;
			return (NULL);
		}
		var_len = count_var_len(s);
		var_name = ft_substr(s, 0, var_len);
		value = find_var(var_name, var_len, data->env);
		free(var_name);
		*len = 1 + var_len; // include the $ in the length
	}
	return (value);
}

/**
 * Counts the length of a variable name, including alphanumeric characters and
 * underscores.
 *
 * This function takes a string representing a variable name and counts the
 * number of characters in the name, including alphanumeric characters and
 * underscores. The function stops counting when it encounters a character that
 * is not alphanumeric or an underscore.
 *
 * @param var A pointer to a null-terminated string representing the variable
 * name.
 * @return The length of the variable name, including alphanumeric characters
 * and underscores.
 */
