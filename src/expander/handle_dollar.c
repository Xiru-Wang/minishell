/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:18:57 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/25 20:38:10 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_var_len(char *var)
{
	int	i;

	i = 0;
	while (ft_isalnum(var[i]) || var[i] == '_')
		i++;
	return (i);
}

char	*handle_dollar(char *s, int *i, char **env, t_data *data)
{
	int		k;
	char	*value;

	k = 0;
	value = expand_dollar(s, &k, env, data);
	if (value == NULL)
		value = ft_strdup("");
	*i += k;
	return (value);
}

//string starts from dollar sign
// call this function only if *s == '$'
char	*expand_dollar(char *s, int *len, char **env, t_data *data)
{
	int		var_len;
	char	*var_name;
	char	*value;

	value = NULL;
	s++;
	if (*s == '?')
	{
		value = ft_itoa(data->exit_code);
		*len = 2;
	}
	else if (ft_isdigit(*s))
	{
		*len = 2;
		return (NULL);
	}
	else
	{
		var_len = count_var_len(s);
		var_name = ft_substr(s, 0, var_len);
		value = find_env(var_name, env);
		free(var_name);
		*len = 1 + var_len;
	}
	return (value);
}
