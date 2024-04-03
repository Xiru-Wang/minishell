/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dollar_quo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/03 20:56:59 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_valid_dollar(char *s);
static char	*replace_vars(char *s, t_data *data);
static int	count_var_len(char *var);
static char	*replace_vars(char *s, t_data *data);

char	*check_dollar_quo(char *s, t_data *data, enum s_type type)//should not free s here?
{
	char	*temp;
	char	*new;
	
	if (check_valid_dollar(s) == 0 && type == WORD) // if no valid dollar: do nothing
		return (ft_strdup(s));
	else if (check_valid_dollar(s) == 0 && type == QUO)
		return(remove_quo(s));
	else
	{
		temp = replace_vars(s, data);
		if (type == QUO)
		{
			new = remove_quo(temp);
			free(temp);
			return(new);
		}
		return (temp);	
	}
}

static int	check_valid_dollar(char *s)
{
	while (*s)
	{
		if (*s == '$')
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
static char	*replace_vars(char *s, t_data *data)//keep quotes, expand var first
{
	int		i, k, j, var_len;
	char	*new, *var_name, *value;

	i = 0;
	k = 0;
	new = (char *)malloc(1024);
	while (s[i])
	{
		if (s[i] == '\'')//no expansion inside of ' '
		{
			new[k] = s[i];
			i++;
			while (s[i] && s[i] != '\'')
				new[k++] = s[i++];//copy content between S_QUO
			i++; 
		}
		else if (s[i] == '$' && s[i + 1] && (ft_isalnum(s[i + 1]) || s[i + 1] == '_'))
		{
			var_len = count_var_len(s + i);
			value = expander(s + i, var_len, data);
			j = 0;
			while(value)
				new[k++] = value[j++];
			free(value);
			i = i + var_len;
		}
		new[k++] = s[i++];
	}
	new[k + 1] = 0;
	return (new);
}

static int	count_var_len(char *var)
{
	int	i;
	
	if (var[0] == '$')
	{
		i = 1;
		while (ft_isalnum(var[i]) || var[i] == '_')
			i++;
	}
	return (i);
}

/*
if check_valid_dollar == true:
situation1: remove
situation2: replace

static char	*get_new_s(char *s, t_data *data)
{
	char	*current;
	char	*new;
	int		i;

	i = 0;
	current = s;
	while (current[i])
	{
		if (current[i] == '$')
		{
			if (ft_isdigit(new[i + 1]))
				new = remove_char(s, i);
			else if (ft_isalpha(s[i + 1]) || s[i + 1] == '_')
				new = replace_var(s, &i, data);//if var found i increase len of env
			if (new && new != current)//if more than one $
			{
				if (current != s)
					free(current);
			}
			current = new;
		}
		i++;
	}
	return (current);
}
*/
/*
static char	*remove_char(char *s, int index)
{
	int		i, k;
	char	*new;

	new = (char *)malloc(ft_strlen(s) - 2 + 1);
	i = 0;
	k = 0;
	while (s[i])
	{
		if (i == index)
			i = i + 2;//skip $ sign and 1 digit
		new[k] = s[i];
		k++;
		i++;
	}
	new[k] = 0;
	return (new);
}
*/
/*
static char	*replace_var(char *s, int *i, t_data *data)
{
	char	*s1, *s2;
	char	*value, *var_name, *new, *temp;
	int		start, end;
	int		len;

	start = *i + 1;//skip $
	len = 0;
	while (ft_isalnum(s[start + len]) || s[start + len] == '_')
		len++;//USER:4
	end = start + len;// 11
	var_name = ft_substr(s, start, len);//USER
	value = find_var(var_name, data->env);//replace USER
	free(var_name);
	s1 = ft_substr(s, 0, *i);// 6chars:"hello*"
	s2 = ft_substr(s, end, ft_strlen(s) - end);//14-11=3char:"*42"
	if (!value) // not exsit, remove $var_name
		new = ft_strjoin(s1, s2);
	else
	{
		temp = ft_strjoin(s1, value);
		new = ft_strjoin(temp, s2);
		free(temp);
		*i += ft_strlen(value);
		free(value);
	}
	free(s1);
	free(s2);
	return (new);
}
*/
/*
eg. "hello*$USER*42" * . () ！etc,will be sepreator
     |  s1 |var|s2|
start = 6: $
len(var) = 5
end = 11
strlen(s)=14
*/