/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:31:14 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 20:34:29 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**reset_env(t_data *data, char **env, int k);

int	call_unset(t_cmd *cmd, t_data *data)
{
	int		i;
	int		k;
	char	*temp;

	i = 1;
	if (!cmd->s[1])
		return (EXIT_SUCCESS);
	while (cmd->s[i])
	{
		temp = ft_strjoin(cmd->s[i], "=");
		if (!temp)
			return (EXIT_FAILURE);
		k = 0;
		while (data->env[k])
		{
			if (ft_strncmp(data->env[k], temp, ft_strlen(temp)) == 0)
			{
				data->env = reset_env(data, data->env, k);
				break ;
			}
			k++;
		}
		free(temp);
		i++;
	}
	return (EXIT_SUCCESS);
}

static char	**reset_env(t_data *data, char **env, int k)
{
	int		i;
	int		j;
	char	**new_env;

	(void) data;
	i = 0;
	j = 0;
	while (env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	if (!new_env)
		return (env);
	i = 0;
	while (env[i])
	{
		if (i != k)
		{
			new_env[j] = env[i];
			j++;
		}
		else
			free(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free(env);
	return (new_env);
}
