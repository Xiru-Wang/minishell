/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 15:22:28 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/06 18:09:31 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_cmd(t_data *data, t_cmd *cmd)
{
	char	*path;
	pid_t pid;
	int status;

	path = find_path(cmd->s[0], data->env);
	if (!path)
	{
		write(STDERR_FILENO, cmd->s[0], ft_strlen(cmd->s[0]));
		write(STDERR_FILENO, ":command not found\n", 19);
		exit(127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		// Child process
		execve(path, cmd->s, data->env);
		// execve only returns if an error occurred
		perror("execve");
		free(path);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Parent process
		waitpid(pid, &status, 0);
		free(path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (EXIT_FAILURE);
	}
}

char	*find_path(char *s, char **env)
{
	char	**paths;
	char	*temp;
	char	*cmd_path;
	int		i;

	i = 0;
	while (ft_strnstr(env[i], "PATH=", 5) == NULL)
		i++;
	paths = ft_split(env[i] + 5, ':');
	i = -1;
	while(paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(temp, s);
		free(temp);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free(paths);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free_double_ptr(paths);
	return (NULL);
}
