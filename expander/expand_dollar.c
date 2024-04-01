/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:20 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/01 18:25:58 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	check_valid_dollar(char *s);
static char	*get_new_s(char *s, t_data *data);
static char	*remove_char(char *s, int index);
static char	*replace_var(char *s, int *start, t_data *data);
static char	*find_var(char *var, char **env);

char	*handle_dollar(char *s, t_data *data)
{
	char	*new;

	if (check_valid_dollar(s) == 0) // if no valid dollar: do nothing
		return (ft_strdup(s));//didnt remove quotes
	else
	{
		new = get_new_s(s, data);
		if (new != s)
			free(s);
		return (new);
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

/*
if check_valid_dollar == true:
situation1: remove
situation2: replace
*/
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
/*
eg. "hello*$USER*42" * . () ！etc,will be sepreator
     |  s1 |var|s2|
start = 6: $
len(var) = 5
end = 11
strlen(s)=14
*/
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

static char	*find_var(char *var, char **env)//eg $USER
{
	char	*value;
	int		i;
	int		n;

	if (!var)
		return (NULL);
	n = ft_strlen(var);//eg.USER
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(var, env[i], n) == 0 && env[i][n] == '=')
		{
			value = ft_substr(env[i], n + 1, ft_strlen(env[i]) - (n + 1));//extract chars after = sign
			return (value);
		}
		i++;
	}
	return (NULL);
}
