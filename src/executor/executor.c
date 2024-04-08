/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:50 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/08 22:28:35 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//if outfile exsit, data goes to file instead of pipe
/*
The strategy for your shell is to have the parent process
do all the piping and redirection before forking the processes.
In this way the children will inherit the redirection.
*/
static int	call_parent(t_cmd *cmd, t_data *data);
static int	pipe_wait(int *pid, int pipe_num);

int executor(t_cmd *cmd, t_data *data)
{
	int		i;
	int		end[2];

	i = 0;
	while (cmd->next)
	{
		if (pipe(end) == -1)
			free_exit("pipe failed", data, STDERR_FILENO);
		check_hd(cmd);
		get_redir_fd_array(cmd);
		redirect_fds(cmd, end);
		data->pid[i] = fork();
		close(end[1]);
		if (data->pid[i] == 0)
		{
			close(end[0]);
			if (cmd->is_builtin)
				call_builtin(cmd);
			else
				call_cmd(data, cmd);
		}
		else
		{
			close(end[1]);
			cmd = cmd->next;
			i++;
		}
	}
	pipe_wait(data->pid, (data->cmd_num - 1));
	call_parent(cmd, data);
	return (0);
}

static int	call_parent(t_cmd *cmd, t_data *data)
{
	check_hd(cmd);
	get_redir_fd_array(cmd);
	if (cmd->is_builtin)
		call_builtin(cmd);
	else
		call_cmd(data, cmd);
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
