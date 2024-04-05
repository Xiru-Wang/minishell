/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:34:04 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/05 18:41:54 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_var_value(char *s, t_data *data);//char *s = $USER
static char	*expand_var(char *var, char **env);

char	*expander(char *s, int len, t_data *data)
{
	char	*var_name;
	char	*value;
	
	var_name = ft_substr(s, 0, len);
	value = get_var_value(var_name, data);
	free(var_name);
	return (value);
}

static char	*get_var_value(char *s, t_data *data)//char *s = $USER
{
	char	*new;

	if (s[0] == '$' && s[1])
	{
		if (ft_isdigit(s[1]))
			new = ft_substr(s, 2, ft_strlen(s) - 2); //$123 -> 23
		else if (ft_isalpha(s[1]) || s[1] == '_')
			new = expand_var(s, data->env);//if var found i increase len of env
	}
	return (new);
}

static char	*expand_var(char *var, char **env)
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
			value = ft_substr(env[i], n + 1, ft_strlen(env[i]) - (n + 1));//get chars after = sign
			return (value);
		}
		i++;
	}
	return (NULL);
}
