/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:50 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/27 18:04:40 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//if outfile exsit, data goes to file instead of pipe
/*
The strategy for your shell is to have the parent process
do all the piping and redirection before forking the processes.
In this way the children will inherit the redirection.
*/

int	g_exit_code;
static int	pipe_wait(int *pid, int pipe_num);

int	executor(t_cmd *cmd, t_data *data)
{
	int	i;
	int	end[2];

	i = 0;
	while (cmd)
	{
		if (cmd->next)
			if (pipe(end) == -1)//create pipe in parent
				free_exit("pipe failed", data, STDERR_FILENO);
		printf("here1\n");
		check_hd(cmd);
		printf("here2\n");
		get_redir_fd_array(cmd);
		printf("here3\n");
		redirect_fds(cmd, end);
		printf("before fork\n");
		data->pid[i] = fork();
		printf("pid[%d]: %d\n", i, data->pid[i]);
		if (data->pid[i] == 0)
		{
			close(end[0]);
			
			if (cmd->is_builtin)
				call_builtin(cmd);
			else
				call_cmd(data, cmd);
			printf("here5\n");
		}
		close(end[1]);
		cmd = cmd->next;
		i++;
	}
	pipe_wait(data->pid, (data->cmd_num - 1));
	return (0);
}

static int	pipe_wait(int *pid, int pipe_num)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipe_num)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	waitpid(pid[i], &status, 0);
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	return (EXIT_SUCCESS);
}
