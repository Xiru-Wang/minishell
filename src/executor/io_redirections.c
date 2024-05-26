/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 17:35:45 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 17:45:13 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	redirect_fdin(t_cmd *cmd, t_io *io);
static void	redirect_fdout(t_cmd *cmd, t_io *io);

void	redirect_io(t_cmd *cmd)
{
	t_io	*io;

	if (!cmd->io_list)
		return ;
	io = cmd->io_list;
	while (io)
	{
		if (io->type == REDIR_IN || io->type == HEREDOC)
			redirect_fdin(cmd, io);
		else if (io->type == REDIR_OUT || io->type == APPEND)
			redirect_fdout(cmd, io);
		if (cmd->err)
		{
			cmd->data->exit_code = 1;
			return ;
		}
		io = io->next;
	}
}

//cat <infile
static void	redirect_fdin(t_cmd *cmd, t_io *io)
{
	int	fd;

	fd = -1;
	if (io->type == REDIR_IN)
		fd = open(io->filename, O_RDONLY);
	else if (io->type == HEREDOC)
		fd = open(io->hdfile, O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			print_error("minishell: ", io->filename, \
					": Permission denied\n");
		else if (errno == ENOENT)
			print_error("minishell: ", io->filename, \
					": No such file or directory\n");
		else
			perror("minishell");
		cmd->err = 1;
		return ;
	}
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

static void	redirect_fdout(t_cmd *cmd, t_io *io)
{
	int	fd;

	fd = -1;
	if (io->type == REDIR_OUT)
		fd = open(io->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (io->type == APPEND)
		fd = open(io->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		if (errno == EACCES)
			print_error("minishell: ", io->filename, \
					": Permission denied\n");
		else if (errno == ENOENT)
			print_error("minishell: ", io->filename, \
					": No such file or directory\n");
		else
			perror("minishell");
		cmd->err = 1;
		return ;
	}
	if (fd != -1)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	backup_stdio(t_cmd *cmd)
{
	cmd->stdin_backup = dup(STDIN_FILENO);
	cmd->stdout_backup = dup(STDOUT_FILENO);
}

void	reset_stdio(t_cmd *cmd)
{
	if (cmd->stdout_backup != -1)
	{
		dup2(cmd->stdout_backup, STDOUT_FILENO);
		close(cmd->stdout_backup);
		cmd->stdout_backup = -1;
	}
	if (cmd->stdin_backup != -1)
	{
		dup2(cmd->stdin_backup, STDIN_FILENO);
		close(cmd->stdin_backup);
		cmd->stdin_backup = -1;
	}
}
