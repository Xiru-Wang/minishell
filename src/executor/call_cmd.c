/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 15:22:28 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/01 17:41:27 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
// Function to find the full path of a command
char *find_command_path(char *cmd, char **env) {
	char *path = getenv("PATH");
	char *path_value = strdup(path);
	char *token = strtok(path_value, ":");
	char *full_path = NULL;

	while (token != NULL) {
		full_path = malloc(strlen(token) + strlen(cmd) + 2);
		strcpy(full_path, token);
		strcat(full_path, "/");
		strcat(full_path, cmd);

		if (access(full_path, X_OK) == 0) {
			free(path_value);
			return full_path;
		}

		free(full_path);
		token = strtok(NULL, ":");
	}

	free(path_value);
	return NULL;
}

// Function to execute a single command
int execute_command(char *cmd, char **args, char **envp) {
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		// Child process
		if (execve(cmd, args, envp) == -1) {
			perror("execve");
			exit(EXIT_FAILURE);
		}
	} else {
		// Parent process
		do {
			if (waitpid(pid, &status, WUNTRACED) == -1) {
				perror("waitpid");
				exit(EXIT_FAILURE);
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return WEXITSTATUS(status);
}
*/

int	call_cmd(t_data *data, t_cmd *cmd)
{
	char	*path;

	path = find_path(cmd->s[0], data->env);
	if (!path)
	{
		write(STDERR_FILENO, cmd->s[0], ft_strlen(cmd->s[0]));
		write(STDERR_FILENO, ":command not found\n", 19);
		exit(127);
	}
	execve(path, cmd->s, data->env);
	free(path);
	free_exit("execve", data, STDERR_FILENO);
	return (0);
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
		if (access(cmd_path, F_OK | X_OK) == 0)//existence || execute/search
		{
			free(paths);
			return (cmd_path);//success
		}
		free(cmd_path);
	}
	free_double_ptr(paths);
	return (NULL);
}
