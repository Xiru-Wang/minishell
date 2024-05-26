/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export_utils_update_var.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:27:08 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/26 16:36:32 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include <stdlib.h>
#include <string.h>

static int	update_var(t_data *data, char *str, int i);

int	handle_quotes_and_check(char *str, int pos)
{
	if (str[pos + 1] == '\"' || str[pos + 1] == '\'')
		delete_quotes(str + pos + 1, str[pos + 1]);
	return (EXIT_SUCCESS);
}

int	find_var_position(t_data *data, char *str, int pos)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], str, pos) == 0)
			return (update_var(data, str, i));
		i++;
	}
	return (-1);
}

static int	update_var(t_data *data, char *str, int i)
{
	free(data->env[i]);
	data->env[i] = ft_strdup(str);
	if (data->env[i] == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	add_new_var(t_data *data, char *str)
{
	char	**new_env;

	new_env = add_var(data->env, str);
	if (!new_env)
		return (EXIT_FAILURE);
	if (data->env != NULL)
		free_double_ptr(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}

int	update_or_add_var(t_data *data, char *str)
{
	int	pos;
	int	result;

	pos = equal_sign(str);
	if (pos == -1)
		return (add_new_var(data, str));
	else if (handle_quotes_and_check(str, pos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	result = find_var_position(data, str, pos);
	if (result != -1)
		return (result);
	return (add_new_var(data, str));
}
