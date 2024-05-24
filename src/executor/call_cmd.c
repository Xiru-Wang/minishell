/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 19:27:42 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/24 12:53:57 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_empty_command(t_cmd *cmd)
{
	return (ft_strncmp(cmd->s[0], "", 1) == 0);
// bash-5.2$ ""
// bash: : command not found
}

static int	wait_for_child(pid_t pid, t_data *data)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		free_exit("waitpid", data, EXIT_FAILURE);
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (EXIT_SIGINT);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_SUCCESS);
}

static void	execute_command(t_cmd *cmd, t_data *data)
{
	execve(cmd->s[0], cmd->s, data->env);
	free_exit("execve", data, EXIT_FAILURE);
}

int	call_cmd(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (is_empty_command(cmd))
		return (EXIT_SUCCESS);//shoud add one space??
	if (find_executable_and_execute(cmd, data) != 0)
		return (EXIT_CMD_NOT_FOUND);
	pid = fork();
	if (pid == 0)
		execute_command(cmd, data);
	else
		status = wait_for_child(pid, data);
	return (status);
}
