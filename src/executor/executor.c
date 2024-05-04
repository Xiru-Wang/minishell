/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:50 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/05 00:28:44 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//if outfile exsit, data goes to file instead of pipe
/*
The strategy for your shell is to have the parent process
do all the piping and redirection before forking the processes.
In this way the children will inherit the redirection.
*/

static int	pipe_wait(int *pid, int pipe_num);
static int	single_cmd(t_cmd *cmd, t_data *data);
static int	multiple_cmds(t_cmd *cmd, t_data *data);
static int	execute_cmd(t_cmd *cmd, t_data *data);

int	executor(t_cmd *cmd, t_data *data)
{
	if (cmd->next)
	{
		multiple_cmds(cmd, cmd->data);
		pipe_wait(data->pid, (data->cmd_num - 1));
	}
	else
		single_cmd(cmd, data);
	return (0);
}

static int	single_cmd(t_cmd *cmd, t_data *data)
{
	check_hd(cmd);
	get_fds(cmd);
	redirect_io_simple(cmd);
	execute_cmd(cmd, data);
	return (EXIT_FAILURE);
}

static int multiple_cmds(t_cmd *cmd, t_data *data)
{
	int	i;
	int	end[2];

	i = 0;
	while (cmd)
	{
		if (cmd->next)
			if (pipe(end) == -1)
				free_exit("pipe failed", data, STDERR_FILENO);
		check_hd(cmd);
		get_fds(cmd);

		//NOTE: find a way to make this better...
		if (cmd->is_builtin == EXIT)
		{
			call_exit(cmd, data);
			return (1);
		}
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			free_exit("fork failed", data, STDERR_FILENO);
		if (data->pid[i] == 0)
		{
			redirect_io(cmd, end);
			execute_cmd(cmd, data);
		}
		else
		{
			if (cmd->prev)
				close(end[0]);
			if (cmd->next)
				close(end[1]);
		}
		cmd = cmd->next;
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	execute_cmd(t_cmd *cmd, t_data *data)
{
	if (cmd->is_builtin)
		call_builtin(cmd);
	else
	{
		call_cmd(data, cmd);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int pipe_wait(int *pid, int pipe_num)
{
	int i;
	int status;

	i = 0;
	while (i <= pipe_num)
	{
		if (waitpid(pid[i], &status, 0) == -1)
		{
			perror("waitpid");
			return (EXIT_FAILURE);
		}
		if (WIFEXITED(status) && i == pipe_num)
			g_exit_code = WEXITSTATUS(status);
		i++;
	}
	return (EXIT_SUCCESS);
}
