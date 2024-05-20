#include "../../includes/minishell.h"

int	find_executable_and_execute(t_cmd *cmd, t_data *data)
{
	char	*path;
	char	*path_tmp;
	char	**paths;
	int		i;

	i = 0;
	if (cmd->s[0][0] == '/' || (cmd->s[0][0] == '.' && (cmd->s[0][1] == '/' || \
					(cmd->s[0][1] == '.' && cmd->s[0][2] == '/'))))
	{
		if (access(cmd->s[0], X_OK) == 0)
			return (0);
		else
		{
			write(STDERR_FILENO, cmd->s[0], strlen(cmd->s[0]));
			write(STDERR_FILENO, ": command not found\n", 20);
			return (EXIT_CMD_NOT_FOUND);
		}
	}
	while (data->env[i] && ft_strncmp(data->env[i], "PATH=", 5) != 0)
		i++;
	if (data->env[i] != NULL)
	{
		paths = ft_split(data->env[i] + 5, ':');
		i = 0;
		while (paths && paths[i])
		{
			path_tmp = ft_strjoin(paths[i], "/");
			path = ft_strjoin(path_tmp, cmd->s[0]);
			free(path_tmp);
			if (access(path, X_OK) == 0)
			{
				if (cmd->s[0])
					free(cmd->s[0]);
				cmd->s[0] = path;
				free_double_ptr(paths);
				return (EXIT_SUCCESS);
			}
			free(path);
			i++;
		}
		free_double_ptr(paths);
	}
	write(STDERR_FILENO, cmd->s[0], ft_strlen(cmd->s[0]));
	write(STDERR_FILENO, ": command not found\n", 20);
	return (EXIT_CMD_NOT_FOUND);
}

int	call_cmd(t_data *data, t_cmd *cmd)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (ft_strncmp(cmd->s[0], "", 1) == 0)
		return (EXIT_SUCCESS);
	if (find_executable_and_execute(cmd, data) != 0)
		return (EXIT_CMD_NOT_FOUND);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd->s[0], cmd->s, data->env);
		free_exit("execve", data, EXIT_FAILURE);
	}
	else
	{
		while (waitpid(pid, &status, 0) == -1)
		{
			if (errno == EINTR)
				continue;
			free_exit("waitpid", data, EXIT_FAILURE);
		}
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				return (EXIT_SIGINT);
		}
		else if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (0);
}
