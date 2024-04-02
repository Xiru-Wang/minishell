#include "../includes/minishell.h"

static int	get_infd(char *s);
static int	get_outfd(t_io *redir);
static void	close_fds(t_cmd *cmd);

//loop io_list to create fdin arrays and fdout arrays
//data stream will only go into the last fd
void get_redir_fd_array(t_cmd *cmd)
{
	t_io	*temp;
	int i;
	int k;

	i = 0;
	k = 0;
	temp = cmd->io_list;
	while (temp)
	{
		if (temp->type == REDIR_IN ||temp->type == HEREDOC)
		{
			cmd->infd[i] = get_infd(temp->filename);
			cmd->last_fdin = i;
			i++;
		}
		else if (temp->type == REDIR_OUT || temp->type == APPEND)
		{
			cmd->outfd[k] = get_outfd(temp);
			cmd->last_fdout = k;
			k++;
		}
		temp = temp->next;
	}
	cmd->infd[i] = 0;
	cmd->infd[k] = 0;
}

//infile has priority, if no infile, check pipe
void	redirect_fds(t_cmd *cmd, int *end)
{
	if (cmd->infd[cmd->last_fdin])
		dup2(cmd->infd[cmd->last_fdin], STDIN_FILENO);
	else if (cmd->prev)
		dup2(end[0], STDIN_FILENO);
	close(end[0]);
	if (cmd->outfd[cmd->last_fdout])
		dup2(cmd->outfd[cmd->last_fdout], STDOUT_FILENO);
	else if (cmd->next)
		dup2(end[1], STDOUT_FILENO);
	close(end[1]);
	close_fds(cmd);//close all opened infiles & outfiles
}

static void	close_fds(t_cmd *cmd)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (i <= cmd->last_fdin)
	{
		close(cmd->infd[i]);
		i++;
	}
	while (k <= cmd->last_fdout)
	{
		close(cmd->outfd[k]);
		k++;
	}
}

static int	get_infd(char *s)
{
	int	fd;

	fd = open(s, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: infile: No such file or directory\n",
			STDERR_FILENO);
		return (0);
	}
	return (fd);
}

static int	get_outfd(t_io *redir)
{
	int	fd;

	if (redir->type == APPEND)
		fd = open(redir->filename, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		fd = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: outfile: Error\n", STDERR_FILENO);
		return (0);
	}
	return (fd);
}

