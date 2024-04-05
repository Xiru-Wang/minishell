/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dollar_quo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/05 19:21:39 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_var_len(char *var);
static char	*replace_vars_complex(char *s, t_data *data);
static char *expand_dollar(char *s, int *len, t_data *data);

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
			strlcat(dst, s, i - 1 + 1); // copy before dollar
			var_len = 0;
			value = expand_dollar(s + i, &var_len, data);
			if (value)
			{
				strlcat(dst, value, i + ft_strlen(value) + 1);//copy expaned value
				free(value);
			}
			strlcat(dst, s + i + var_len, i + ft_strlen(s) + ft_strlen(value) - var_len + 1);
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

static int	count_var_len(char *var)
{
	int	i;

	i = 0;
	while (ft_isalnum(var[i]) || var[i] == '_')
		i++;
	return (i);
}
