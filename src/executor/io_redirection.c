#include "../../includes/minishell.h"

static int	get_infd(char *s);
static int	get_outfd(t_io *redir);
//static void	close_fds(t_cmd *cmd);

void	count_fds(t_cmd *cmd)
{
	t_io	*temp;

	temp = cmd->io_list;
	while (temp)
	{
		if (temp->type == REDIR_IN || temp->type == HEREDOC)
			cmd->num_fdin++;
		else if (temp->type == REDIR_OUT || temp->type == APPEND)
			cmd->num_fdout++;
		temp = temp->next;
	}
}

void	get_fds(t_cmd *cmd)
{
	t_io	*temp; // Temporary pointer to traverse the I/O list
	int		i; // Index for input file descriptor array
	int		k; // Index for output file descriptor array

	i = 0;
	k = 0;
	temp = cmd->io_list;
	if (!cmd->io_list)
		return ;
	count_fds(cmd);
	while (temp)
	{
		if (temp->type == REDIR_IN || temp->type == HEREDOC)
		{
			cmd->infd = get_infd(temp->filename);
			i++;
			if (i != cmd->num_fdin)//关闭最后一个fd之前的
				close(cmd->infd);

		}
		else if (temp->type == REDIR_OUT || temp->type == APPEND)
		{
			cmd->outfd = get_outfd(temp);
			k++;
			if (k != cmd->num_fdout)
				close(cmd->outfd);
		}
		temp = temp->next;
	}
}

static void redirect_input(t_cmd *cmd, int *end)
{
	if (cmd->infd != -1)
	{
		dup2(cmd->infd, STDIN_FILENO);
		close(cmd->infd);
	}
	else if (cmd->prev)
	{
		dup2(end[0], STDIN_FILENO);
		close(end[0]);
	}
}

static void redirect_output(t_cmd *cmd, int *end)
{
	if (cmd->outfd != -1)
	{
		dup2(cmd->outfd, STDOUT_FILENO);
		close(cmd->outfd);
	}
	else if (cmd->next)
	{
		dup2(end[1], STDOUT_FILENO);
		close(end[1]);
	}
}

void	redirect_io(t_cmd *cmd, int *end)
{
	if (!cmd->io_list)
		return ;
	redirect_input(cmd, end);
	redirect_output(cmd, end);
}

void	redirect_io_simple(t_cmd *cmd)
{
	if (!cmd->io_list)
		return ;
	//cmd->stdin = dup(STDIN_FILENO);
	//cmd->stdout = dup(STDOUT_FILENO);
	if (cmd->infd != -1)
	{
		dup2(cmd->infd, STDIN_FILENO);
		close(cmd->infd);
	}
	if (cmd->outfd != -1)
	{
		dup2(cmd->outfd, STDOUT_FILENO);
		close(cmd->outfd);
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
		return 0;
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
		return 0;
	}
	return (fd);
}
