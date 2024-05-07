/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:50 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/06 18:07:36 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	pipe_wait(int *pid, int pipe_num);
static int	single_cmd(t_cmd *cmd, t_data *data);
static int	multiple_cmds(t_cmd *cmd, t_data *data);
static int	execute_cmd(t_cmd *cmd, t_data *data);

int	executor(t_cmd *cmd, t_data *data)
{
	data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
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
	int exit_status;

	check_hd(cmd);
	get_fds(cmd);
	redirect_io_simple(cmd);
	exit_status = execute_cmd(cmd, data);
	//reset_stdio(cmd);
	return (exit_status);
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
		// if (cmd->is_builtin == EXIT)
		// {
		// 	call_exit(cmd, data);
		// 	return (1);
		// }
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			free_exit("fork failed", data, STDERR_FILENO);
		if (data->pid[i] == 0)
		{
			redirect_io(cmd, end);
			//reset_stdio(cmd);
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
	{
		call_builtin(cmd);
		reset_stdio(cmd);
	}		
	else
	{
		call_cmd(data, cmd);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	pipe_wait(int *pid, int pipe_num)
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
