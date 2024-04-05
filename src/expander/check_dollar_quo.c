/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dollar_quo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/05 21:20:01 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_var_len(char *var);
static char *expand_dollar(char *s, int *len, t_data *data);
static int	len_single_quo(char *s);

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

// remove: $123 -> 23
// replace/delete $USER/$hiii
//check whole string if there's any valid sign to remove/expand
int	check_valid_dollar(char *s)
{
	while (*s)
	{
		if (*s == '$' && *(s + 1))
		{
			s++;
			if (*s && (ft_isalnum(*s) || *s == '_'))
				return (1);
		}
		s++;
	}
	return (0);
}

// this function should handle this case
// in: bla'$USER'waw"$USER""$?"over
// out: bla'$USER'wawUSERNAME0over

/**
 * Replaces variables in a given string with their corresponding values from a
 * provided data structure.
 *
 * This function takes a string `s` and a pointer to a `t_data` structure `data`
 * as input. It iterates through the characters of the input string `s`, looking
 * for variables enclosed in '$' symbols. When a variable is found, it replaces
 * it with its corresponding value from the `data` structure. The resulting
 * string with replaced variables is returned.
 *
 * @param s The input string in which variables need to be replaced
 * @param data A pointer to a `t_data` structure containing variable-value pairs
 * @return A new string with variables replaced by their corresponding values
 */
char	*replace_vars_complex(char *s, t_data *data)
{
	int		i, var_len, flag;
	char	*dst, *value;

	flag = 0;
	dst = ft_calloc(1, 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			i += len_single_quo(s + i);
		else if (s[i] == '$' && check_valid_dollar(s + i))
		{
			flag = 1;
			ft_strlcat(dst, s, ft_strlen(dst) + i + 1);
			var_len = 0;
			value = expand_dollar(s + i, &var_len, data);
			if (value)
			{
				ft_strlcat(dst, value, ft_strlen(dst) + ft_strlen(value) + 1);
				free(value);
			}
			ft_strlcat(dst, s + i + var_len, ft_strlen(dst) + ft_strlen(s + i + var_len) + 1);
			//copy the rest
			i = i + var_len;
		}
		i++;
	}
	if (flag)
		return (dst);
	free(dst);
	return (ft_strdup(s));
}

//count chars from 1st s_quo to next s_quo
static int	len_single_quo(char *s)
{
	int	len;

	len = 0;
	if (*s == '\'') // len_s_quo
	{
		len++;
		while (*s && *s != '\'')
			len++;
		if (*s == '\'')
			len++;
	}
	return (len);
}

static char	*expand_dollar(char *s, int *len, t_data *data)
{
	int		var_len;
	char	*var_name;
	char	*value;

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
		return (value);
	}
	return (NULL);
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
static int count_var_len(char *var)
{
    int i;

    i = 0;
    while (ft_isalnum(var[i]) || var[i] == '_')
        i++;
    return (i);
}
