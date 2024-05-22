/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export_add_var.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:27:08 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/22 08:00:17 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function prototypes
static char	**cleanup_new_env(char **new_env, int i);

int	get_env_len(char **env)
{
	int	len;

	len = 0;
	while (env[len])
		len++;
	return (len);
}

char	**allocate_new_env(int len)
{
	char	**new_env;

	new_env = (char **)malloc(sizeof(char *) * len);
	return (new_env);
}

int	copy_env_vars(char **new_env, char **env, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new_env[i] = strdup(env[i]);
		if (new_env[i] == NULL)
		{
			cleanup_new_env(new_env, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static char	**cleanup_new_env(char **new_env, int i)
{
	while (i > 0)
		free(new_env[--i]);
	free(new_env);
	return (NULL);
}

char	**add_str_to_env(char **new_env, char *str, int len)
{
	new_env[len] = strdup(str);
	if (new_env[len] == NULL)
		return (cleanup_new_env(new_env, len));
	new_env[len + 1] = NULL;
	return (new_env);
}
