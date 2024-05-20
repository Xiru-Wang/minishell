/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:27:08 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 20:30:53 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	equal_sign(char *str)
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

static int	is_valid_identifier(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	export_error(char *str)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (EXIT_FAILURE);
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

static	char	**add_var(char **env, char *str)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	i = 0;
	while (env[len])
		len++;
	new_env = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (NULL);
	while (i < len)
	{
		new_env[i] = ft_strdup(env[i]);
		if (new_env[i] == NULL)
		{
			while (i > 0)
				free(new_env[--i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[len] = ft_strdup(str);
	if (new_env[len] == NULL)
	{
		while (--i >= 0)
			free(new_env[i]);
		free(new_env);
		return (NULL);
	}
	new_env[len + 1] = NULL;
	return (new_env);
}

static int	update_or_add_var(t_data *data, char *str)
{
	int		pos;
	int		i;
	char	**new_env;

	pos = equal_sign(str);
	if (pos == -1)
		return (EXIT_FAILURE);
	if (str[pos + 1] == '\"' || str[pos + 1] == '\'')
		delete_quotes(str + pos + 1, str[pos + 1]);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], str, pos) == 0)
		{
			free(data->env[i]);
			data->env[i] = ft_strdup(str);
			if (data->env[i] == NULL)
				return (EXIT_FAILURE);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	new_env = add_var(data->env, str);
	if (!new_env)
		return (EXIT_FAILURE);
	if (data->env != NULL)
		free_double_ptr(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
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
