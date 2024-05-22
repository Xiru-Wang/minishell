/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:27:08 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/22 08:12:22 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	equal_sign(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		return (i);
	return (-1);
}

void	delete_quotes(char *str, char quote)
{
	char	*read;
	char	*write;

	read = str;
	write = str;
	while (*read)
	{
		if (*read != quote)
			*write++ = *read;
		read++;
	}
	*write = '\0';
}

static int	check_parameter(char *str)
{
	int	i;

	if (ft_isdigit(str[0]) || str[0] == '=')
		return (export_error(str));
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!is_valid_identifier(str[i]))
			return (export_error(str));
		i++;
	}
	if (str[i] == '=')
		return (EXIT_SUCCESS);
	return (export_error(str));
}

char	**add_var(char **env, char *str)
{
	int		len;
	char	**new_env;

	len = get_env_len(env);
	new_env = allocate_new_env(len + 2);
	if (!new_env)
		return (NULL);
	if (!copy_env_vars(new_env, env, len))
		return (NULL);
	new_env = add_str_to_env(new_env, str, len);
	return (new_env);
}

int	call_export(t_cmd *cmd, t_data *data)
{
	int	i;

	if (!cmd->s[1])
	{
		i = 0;
		while (data->env && data->env[i])
		{
			ft_putstr_fd(data->env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			i++;
		}
		return (EXIT_SUCCESS);
	}
	i = 1;
	while (cmd->s[i])
	{
		if (check_parameter(cmd->s[i]) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (update_or_add_var(data, cmd->s[i]) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
