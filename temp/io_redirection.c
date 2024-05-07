#include "../../includes/minishell.h"

static int	get_infd(char *s);
static int	get_outfd(t_io *redir);

// void	count_fds(t_cmd *cmd)
// {
// 	t_io	*temp;

// 	temp = cmd->io_list;
// 	while (temp)
// 	{
// 		if (temp->type == REDIR_IN || temp->type == HEREDOC)
// 			cmd->num_fdin++;
// 		else if (temp->type == REDIR_OUT || temp->type == APPEND)
// 			cmd->num_fdout++;
// 		temp = temp->next;
// 	}
// }

void get_fds(t_cmd *cmd)
{
	t_io *temp;

	if (!cmd->io_list)
		return;
	//count_fds(cmd);
	temp = cmd->io_list;
	while (temp)
	{
		if (temp->type == REDIR_IN)
		{
			if (cmd->infd != -1)
				close(cmd->infd);
			cmd->infd = get_infd(temp->filename);
		}
		else if (temp->type == REDIR_IN || temp->type == HEREDOC)
		{
			if (cmd->infd != -1)
				close(cmd->infd);
			cmd->infd = get_infd(cmd->hdfile);
		}
		else if (temp->type == REDIR_OUT || temp->type == APPEND)
		{
			if (cmd->outfd != -1)
				close(cmd->outfd);
			cmd->outfd = get_outfd(temp);
		}
		temp = temp->next;
	}
}

static void redirect_input(t_cmd *cmd, int *end)
{
	if (cmd->infd != -1)
	{
		if (dup2(cmd->infd, STDIN_FILENO) == -1)
		{
			perror("dup2 failed on input redirection");
			close(cmd->infd); // Attempt to close if dup2 fails
			return;
		}
		close(cmd->infd);
		if (cmd->hdfile)//NOT SURE
			unlink(cmd->hdfile);
	}
	else if (cmd->prev && end)
	{
		if (dup2(end[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed on pipe input redirection");
			close(end[0]); // Attempt to close if dup2 fails
			return;
		}
		close(end[0]);
	}
}

static void	redirect_output(t_cmd *cmd, int *end)
{
	if (cmd->outfd != -1)
	{
		if (dup2(cmd->outfd, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed on output redirection");
			close(cmd->outfd); // Attempt to close if dup2 fails
			return;
		}
	}
	else if (cmd->next && end)
	{
		if (dup2(end[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed on pipe output redirection");
			close(end[1]); // Attempt to close if dup2 fails
			return;
		}
	}
}

void	redirect_io(t_cmd *cmd, int *end)
{
	if (!cmd->io_list)
		return ;
	cmd->stdin_backup = dup(STDIN_FILENO);
	cmd->stdout_backup = dup(STDOUT_FILENO);
	redirect_input(cmd, end);
	redirect_output(cmd, end);
}

void	redirect_io_simple(t_cmd *cmd)
{
	if (!cmd->io_list)
		return ;
	cmd->stdin_backup = dup(STDIN_FILENO);
	cmd->stdout_backup = dup(STDOUT_FILENO);
	if (cmd->infd != -1)
	{
		dup2(cmd->infd, STDIN_FILENO);
		close(cmd->infd);
		if (cmd->hdfile)//NOT SURE
			unlink(cmd->hdfile);
	}
	if (cmd->outfd != -1)
	{
		dup2(cmd->outfd, STDOUT_FILENO);
		close(cmd->outfd);
	}
}

void reset_stdio(t_cmd *cmd)
{
	// Restore stdin from backup if valid
	if (cmd->stdin_backup != -1)
	{
		if (dup2(cmd->stdin_backup, STDIN_FILENO) == -1)
			perror("dup2 failed to restore stdin");
		if (close(cmd->stdin_backup) == -1)
			perror("close failed on stdin_backup");
		cmd->stdin_backup = -1; // Invalidate the backup descriptor
	}
	// Restore stdout from backup if valid
	if (cmd->stdout_backup != -1)
	{
		if (dup2(cmd->stdout_backup, STDOUT_FILENO) == -1)
			perror("dup2 failed to restore stdout");
		if (close(cmd->stdout_backup) == -1)
			perror("close failed on stdout_backup");
		cmd->stdout_backup = -1; // Invalidate the backup descriptor
	}
}

static int	get_infd(char *s)
{
	int fd;

	fd = open(s, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: infile: No such file or directory\n", \
				STDERR_FILENO);
		return -1;
	}
	return (fd);
}

static int	get_outfd(t_io *redir)
{
	int fd;

	if (redir->type == APPEND)
		fd = open(redir->filename, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		fd = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: outfile: Error\n", STDERR_FILENO);
		return -1;
	}
	return (fd);
}
