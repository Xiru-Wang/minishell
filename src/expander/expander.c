/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:34:04 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/05 19:59:10 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expander(char *s, int len, t_data *data)
{
	char	*var_name;
	char	*value;

	var_name = ft_substr(s, 0, len);
	value = find_var(var_name, len, data->env);
	free(var_name);
	return (value);
}

/**
 * Finds the value of a given environment variable in the provided environment
 * array.
 *
 * This function searches for the specified variable in the environment array
 * and returns its value.
 *
 * @param var The variable to search for in the environment array.
 * @param len The length of the variable name.
 * @param env The environment array to search in.
 * @return The value of the variable if found, NULL otherwise.
 */
char	*find_var(char *var, int len, char **env)
{
	char	*value;
	int		i;

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

