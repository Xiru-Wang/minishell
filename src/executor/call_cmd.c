/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 19:27:42 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 19:29:37 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
