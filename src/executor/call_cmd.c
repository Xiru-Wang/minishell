#include "../../includes/minishell.h"

int	find_executable_and_execute(t_cmd *cmd, t_data *data) {
	char *path = NULL;
	char *path_tmp = NULL;
	char **paths;
	int i = 0;

	// First, try to execute the command as is, it might be an absolute or relative path
	if (access(cmd->s[0], X_OK) == 0)
		return 0; // Executable found, ready to execute it in call_cmd
	// If the command is not found, search in the PATH directories
	while (data->env[i] && ft_strncmp(data->env[i], "PATH=", 5) != 0)
		i++;
	if (data->env[i] != NULL)
	{ // PATH found
		paths = ft_split(data->env[i] + 5, ':');
		i = 0;
		while (paths && paths[i]) {
			path_tmp = ft_strjoin(paths[i], "/");
			path = ft_strjoin(path_tmp, cmd->s[0]); // Note: consider optimizing this to avoid memory leak
			free(path_tmp);
			if (access(path, X_OK) == 0)
			{
				cmd->s[0] = path; // Update cmd->s[0] to the full path
				free_double_ptr(paths);
				return 0;
			}
			free(path);
			i++;
		}
		free_double_ptr(paths);
	}
	write(STDERR_FILENO, cmd->s[0], ft_strlen(cmd->s[0]));
	write(STDERR_FILENO, ": command not found\n", 20);
	return (127);
}

int	call_cmd(t_data *data, t_cmd *cmd)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (find_executable_and_execute(cmd, data) != 0)
		return (127);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd->s[0], cmd->s, data->env);
		free_exit("execve", data, EXIT_FAILURE);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			free_exit("waitpid", data, EXIT_FAILURE);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (0);
}
