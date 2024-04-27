/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:50 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/27 20:53:47 by jschroed         ###   ########.fr       */
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
		check_hd(cmd);
		get_redir_fd_array(cmd);
		redirect_fds(cmd, end);
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			free_exit("fork failed", data, STDERR_FILENO);
		// printf("pid[%d]: %d\n", i, data->pid[i]);
		if (data->pid[i] == 0)
		{
			if (!cmd->next)
				close(end[0]);
			if (cmd->prev)
				close(end[1]);

			if (cmd->is_builtin)
				call_builtin(cmd);
			else
			{
				call_cmd(data, cmd);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (cmd->prev)
				close(end[1]);  // Close the write end of the previous command's pipe
			if (cmd->next)
				close(end[0]);  // Close the read end of the current command's pipe
		}
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
	while (i <= pipe_num)
	{
		if (waitpid(pid[i], &status, 0) == -1)
		{
			perror("waitpid");
			return (EXIT_FAILURE);
		}
		if (WIFEXITED(status))
			g_exit_code = WEXITSTATUS(status);
		i++;
	}
	return (EXIT_SUCCESS);
}
