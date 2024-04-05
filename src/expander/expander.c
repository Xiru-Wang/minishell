/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:34:04 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/05 19:37:10 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*find_var(char *var, int len, char **env);

char	*expander(char *s, int len, t_data *data)
{
	char	*var_name;
	char	*value;

	var_name = ft_substr(s, 0, len);
	value = find_var(var_name, len, data->env);
	free(var_name);
	return (value);
}

static char	*find_var(char *var, int len, char **env)
{
	char	*value;
	int		i;
	int		n;

	if (!var)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(var, env[i], len) == 0 && env[i][len] == '=')
		{
			value = ft_substr(env[i], len + 1, ft_strlen(env[i]) - (len + 1));//get chars after = sign
			return (value);
		}
		i++;
	}
	return (NULL);
}
