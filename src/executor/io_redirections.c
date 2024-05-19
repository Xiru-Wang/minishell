/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 17:35:45 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/19 11:43:12 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	redirect_fdin(t_io *io, t_cmd *cmd);
static void	redirect_fdout(t_io *io);

void	redirect_io(t_cmd *cmd)
{
	t_io	*io;

	if (!cmd->io_list)
		return ;
	io = cmd->io_list;
	while (io)
	{
		if (io->type == REDIR_IN || io->type == HEREDOC)
			redirect_fdin(io, cmd);
		else if (io->type == REDIR_OUT || io->type == APPEND)
			redirect_fdout(io);
		io = io->next;
	}
}

static void	redirect_fdin(t_io *io, t_cmd *cmd)
{
	int	fd;

	fd = -1;
	if (io->type == REDIR_IN)
		fd = open(io->filename, O_RDONLY);
	else if (io->type == HEREDOC)
		fd = open(cmd->hdfile, O_RDONLY);
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		if (cmd->hdfile)
			unlink(cmd->hdfile);
	}
}

static void	redirect_fdout(t_io *io)
{
	int	fd;

	fd = -1;
	if (io->type == REDIR_OUT)
		fd = open(io->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (io->type == APPEND)
		fd = open(io->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
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
