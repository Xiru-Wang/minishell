/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 19:27:42 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 19:29:24 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_relative_or_absolute(char *command)
{
	if (command[0] == '/')
		return (EXIT_FAILURE);
	if (command[0] == '.' && command[1] == '/')
		return (EXIT_FAILURE);
	if (command[0] == '.' && command[1] == '.' && command[2] == '/')
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	check_access_and_print_error(char *command)
{
	if (access(command, X_OK) == 0)
		return (EXIT_SUCCESS);
	write(STDERR_FILENO, command, strlen(command));
	write(STDERR_FILENO, ": command not found\n", 20);
	return (EXIT_CMD_NOT_FOUND);
}

static char	**get_paths_from_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] && strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i])
		return (ft_split(env[i] + 5, ':'));
	return (NULL);
}

static int	find_in_paths(char *command, char **paths, t_cmd *cmd)
{
	char	*path;
	char	*path_tmp;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		path_tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_tmp, command);
		free(path_tmp);
		if (access(path, X_OK) == 0)
		{
			free(cmd->s[0]);
			cmd->s[0] = path;
			return (EXIT_SUCCESS);
		}
		free(path);
		i++;
	}
	return (EXIT_CMD_NOT_FOUND);
}

int	find_executable_and_execute(t_cmd *cmd, t_data *data)
{
	char	**paths;
	int		result;

	if (is_relative_or_absolute(cmd->s[0]))
		return (check_access_and_print_error(cmd->s[0]));
	paths = get_paths_from_env(data->env);
	result = find_in_paths(cmd->s[0], paths, cmd);
	free_double_ptr(paths);
	if (result == 0)
		return (EXIT_SUCCESS);
	write(STDERR_FILENO, cmd->s[0], strlen(cmd->s[0]));
	write(STDERR_FILENO, ": command not found\n", 20);
	return (EXIT_CMD_NOT_FOUND);
}