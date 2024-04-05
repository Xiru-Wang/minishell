/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dollar_quo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/05 19:32:45 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_var_len(char *var);
static char	*replace_vars_complex(char *s, t_data *data);
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

// hi"hi"' '"hi"->hihi hi
// echo blabla"waw"'mao'"$USER" ---> blablawawmaoxiruwang
static char	*replace_vars_complex(char *s, t_data *data)// s = $USER
{
	int		i, var_len, flag;
	char	*dst, *value, *s1, *s2;

	flag = 0;
	dst = ft_calloc(1, 1);
	while (s[i])
	{
		if (s[i] == '\'')
			i += len_single_quo(s + i);
		else if (s[i] == '$' && check_valid_dollar(s + i))
		{
			flag = 1;
			ft_strlcat(dst, s, i - 1 + 1); // copy before dollar
			var_len = 0;
			value = expand_dollar(s + i, &var_len, data);
			if (value)
			{
				ft_strlcat(dst, value, i + ft_strlen(value) + 1);//copy expaned value
				free(value);
			}
			ft_strlcat(dst, s + i + var_len, i + ft_strlen(s) + ft_strlen(value) - var_len + 1);
			//copy the rest
			i = i + var_len;
		}
		i++;
	}
	if (flag)
		return (dst);
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
	char	*value;

	if (*s == '$')
	{
		s++;//skip dollar
		if (ft_isdigit(*s)) // skip 1st digit
		{
			*len = 2;
			return (NULL);
		}
		*len = count_var_len(s);
		value = expander(s, len, data);
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


/**
 * Removes single and double quotes from a given string.
 *
 * This function takes a string as input and removes any single or double quotes
 * from it. It allocates memory for a new string without the quotes and returns
 * a pointer to the new string. If memory allocation fails, it returns NULL.
 *
 * @param s The input string from which quotes are to be removed.
 * @return A pointer to the new string without quotes, or NULL if memory
 * allocation fails.
 */
char	*remove_quo(char *s)
{
	int		i;
	int		j;
	int		len;
	char	*new_str;

	len = ft_strlen(s);
	new_str = (char *)malloc(sizeof(char) * (len - 1));
	if (new_str == NULL)
		return (NULL);

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[i] != '"' && s[i] != '\'')
		{
			new_str[j] = s[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';

	return (new_str);
}

