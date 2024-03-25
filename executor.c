/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:50 by xiwang            #+#    #+#             */
/*   Updated: 2024/03/24 19:49:11 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	fork_exev(t_cmd *cmd, t_data *data);

int executor(t_cmd *cmd, t_data *data)
{
	int	end[2];
	//?? check bash's behavior at 42
//	if ((data->infile || cmd->infile) && cmd->id == 0)
//			cmd->infd = open(data->infile, O_RDONLY);
	while (cmd->next != NULL)//create pipe if not the last cmd
	{
		pipe(end);
		cmd->outfd = end[1];//write
		cmd->next->infd = end[0];//read
		fork_exev(cmd, data);
		cmd = cmd->next;//will this line happen?
	}
	if (data->outfile && cmd->next == NULL)//last cmd
	{
		//
	}
}
		// if (pipe(end) == -1)
		// 		free_exit("pipe", data, STDERR);

void	get_outfd(t_cmd *cmd, t_data *data)
{
	if (cmd->o_type == APPEND)
		cmd->outfd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (cmd->o_type == REDIR_OUT)
		cmd->outfd = open(cmd->outfile, O_WRONLY | O_CREAT |O_TRUNC, 0644);
	if (cmd->outfd < 0)
		free_exit("minishell: outfile: Error", data, STDERR_FILENO);
}

void	get_infd(t_cmd *cmd, t_data *data)
{
	if (cmd->in_type == REDIR_IN)
		cmd->infd = open(cmd->infile, O_RDONLY);
	if (data->infile && cmd->id == 0)//??BUG
		cmd->infd = open(data->infile, O_RDONLY);
	else if (cmd->in_type == HEREDOC)
		cmd->infile = cmd->hdfile;/////??????
	if (cmd->infd < 0)
		free_exit("minishell: infile: No such file or directory", data, STDERR_FILENO);
}

void	fork_exev(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	pid_t	last;

	pid = fork();
	if (pid == -1)
		free_exit("fork", data, STDERR_FILENO);
	if (pid > 0)
		last = pid;//parent
	else if (pid == 0)
	{
		if (cmd->infd != STDIN_FILENO)
		{
			dup2(cmd->infd, STDIN_FILENO);
			close(cmd->infd);
		}
		if (cmd->outfd != STDOUT_FILENO)
		{
			dup2(cmd->outfd, STDOUT_FILENO);
			close(cmd->outfd);
		}
		//signal reset??
		call_cmd(data, cmd);
	}
}

// int	executor(t_data *data)
// {
// 	int	fd[2];
// 	int	infile;
// 	int	outfile;

// 	//1: check < infile
// 	if (data->in)
// 		infile = open(data->in, O_RDONLY);
// 	//2: handle  pipes
// 	int i = 0;
// 	//while (i < data->cmd_num)
// 	while (data->cmd_list) // i < pipe_num
// 	{
// 		pipe();
// 	}
// 	//3: check > || >> outfile
// 	if (data->out)
// 		outfile = open(data->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// }
