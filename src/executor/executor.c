/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:35:04 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/27 18:14:05 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	execute_single_command(t_cmd *cmd);
static int	execute_command_pipeline(t_cmd *cmd, int fd_in, int *i);
static int	setup_child_process(t_cmd *cmd, int *end, int fd_in);
static int	wait_for_processes(int *pids, int num_pids);

int	executor(t_cmd *cmd, t_data *data)
{
	int		fd_in;
	int		i;

	i = 0;
	fd_in = STDIN_FILENO;
	data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
	backup_stdio(cmd);
	if (cmd->next == NULL)
		data->exit_code = execute_single_command(cmd);
	else
		data->exit_code = execute_command_pipeline(cmd, fd_in, &i);
	reset_stdio(cmd);
	if (data->pid)
		free(data->pid);
	if (data->exit_code == EXIT_SIGINT)
		return (EXIT_SIGINT);
	else
		return (0);
}

static int	execute_single_command(t_cmd *cmd)
{
	int	status;

	if (check_hd(cmd) == EXIT_SIGINT)
		return (EXIT_SIGINT);
	redirect_io(cmd);
	if (!cmd->s)
	{
		reset_stdio(cmd);
		return (0);
	}
	if (cmd->err)
	{
		reset_stdio(cmd);
		return (1);
	}
	if (cmd->is_builtin)
		status = call_builtin(cmd);
	else
		status = call_cmd(cmd->data, cmd);
	return (status);
}

/*
eg. echo string > file1 | cat --> string goes to file1 not pipe

child one: execute the cmd, write to pipe
parent: close the end[write], keep end[read]=(fd_in) open for next child
child two: read from the pipe
--> parent manage the children and control the pipe

attention: if there's redirections, close pipe, dup2(redirect file, pipe)
*/
static int	execute_command_pipeline(t_cmd *cmd, int fd_in, int *i)
{
	int		end[2];
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (current->next)
			pipe(end);
		if (check_hd(current) == EXIT_SIGINT)
			return (EXIT_SIGINT);
		cmd->data->pid[*i] = fork();
		if (cmd->data->pid[*i] == 0)
			setup_child_process(current, end, fd_in);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (current->next)
		{
			close(end[1]);
			fd_in = end[0];
		}
		current = current->next;
		(*i)++;
	}
	reset_stdio(cmd);
	return (wait_for_processes(cmd->data->pid, cmd->data->cmd_num));
}

static int	setup_child_process(t_cmd *cmd, int *end, int fd_in)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->next)
	{
		close(end[0]);
		dup2(end[1], STDOUT_FILENO);
		close(end[1]);
	}
	redirect_io(cmd);
	if (cmd->err)
		exit(1);
	if (cmd->is_builtin)
		exit(call_builtin(cmd));
	else
		exit(call_cmd(cmd->data, cmd));
}

//echo $? ---->> equal last child exit status🤔
static int	wait_for_processes(int *pids, int num_pids)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	exit_status = 0;
	while (i < num_pids)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	return (exit_status);
}
