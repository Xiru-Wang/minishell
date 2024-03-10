/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:57:24 by xiruwang          #+#    #+#             */
/*   Updated: 2024/03/08 11:35:01 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/*
cmd 1 < file1 << EOF | cmd 2 >> file2
执行顺序
根据上述解释，执行顺序如下：

cmd 1 从 heredoc 读取输入，而不是从 file1。尽管 file1 被指定为输入，但 heredoc (<< EOF) 的存在使得输入来自于 heredoc 的内容。
cmd 1 处理 heredoc 提供的输入，并将其输出传递给 cmd 2。
cmd 2 接收来自 cmd 1 的输出作为输入，处理后，将结果追加到 file2。
*/

void	fork_exev(t_cmd *cmd, t_data *data);

int executor(t_cmd *cmd, t_data *data)
{
	int	end[2];

	assign_cmd_id(cmd);
	if (data->infile && cmd->id == 0)
			cmd->infd = open(data->infile, O_RDONLY);
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
		if (data->append)//append只能最后一条命令？
			cmd->outfd = open(data->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			cmd->outfd = open(data->outfile, O_WRONLY | O_CREAT |O_TRUNC, 0644);
		if (cmd->outfd < 0)
			free_exit("open", data, STDERR_FILENO);
	}
}
		// if (pipe(end) == -1)
		// 		free_exit("pipe", data, STDERR);

void	fork_exev(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	pid_t	last;

	pid = fork();
	if (pid == -1)
		free_exit("fork", data, STDERR);
	if (pid > 0)
		last = pid;//parent
	else if (pid == 0)
	{
		if (cmd->infd != STDIN_FILENO)
		{
			dup2(cmd->infd, STDIN_FILENO);
			close(cmd->infd);//?
		}
		if (cmd->outfd != STDOUT_FILENO)
		{
			dup2(cmd->outfd, STDOUT_FILENO);
			close(cmd->outfd);//?
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
