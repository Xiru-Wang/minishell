#include "../../includes/minishell.h"

static int	get_infd(char *s);
static int	get_outfd(t_io *redir);
static void	close_fds(t_cmd *cmd);

//loop io_list to create fdin arrays and fdout arrays
//data stream will only go into the last fd
/**
 * Populates the input and output file descriptor arrays of a given command
 * structure.
 *
 * This function iterates through the list of I/O redirections in the command
 * structure and assigns file descriptors to the corresponding arrays based on
 * the type of redirection.
 *
 * @param cmd A pointer to the command structure containing the I/O redirection
 * list
 * @return void
 */
void get_redir_fd_array(t_cmd *cmd)
{
	t_io *temp; // Temporary pointer to traverse the I/O list
	int i; // Index for input file descriptor array
	int k; // Index for output file descriptor array

	i = 0;
	k = 0;
	temp = cmd->io_list;

	// Iterate through the I/O list
	while (temp)
	{
		if (temp->type == REDIR_IN || temp->type == HEREDOC)
		{
			cmd->infd[i] = get_infd(temp->filename); // Assign input file descriptor
			cmd->last_fdin = i; // Update last input file descriptor index
			i++;
		}
		else if (temp->type == REDIR_OUT || temp->type == APPEND)
		{
			cmd->outfd[k] = get_outfd(temp); // Assign output file descriptor
			cmd->last_fdout = k; // Update last output file descriptor index
			k++;
		}
		temp = temp->next; // Move to the next I/O redirection
	}
}

void redirect_fds(t_cmd *cmd, int *end)
{
	// Handle input redirection
	if (cmd->last_fdin >= 0 && cmd->infd[cmd->last_fdin] != 0)
	{
		printf("Input redirection: cmd->infd[cmd->last_fdin] = %d\n", cmd->infd[cmd->last_fdin]);
		if (dup2(cmd->infd[cmd->last_fdin], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->infd[cmd->last_fdin]);
	}
	else if (cmd->prev)
	{
		printf("Pipe input: end[0] = %d\n", end[0]);
		if (dup2(end[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(end[0]);
	}

	// Handle output redirection
	if (cmd->last_fdout >= 0 && cmd->outfd[cmd->last_fdout] != 0)
	{
		printf("Output redirection: cmd->outfd[cmd->last_fdout] = %d\n", cmd->outfd[cmd->last_fdout]);
		if (dup2(cmd->outfd[cmd->last_fdout], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->outfd[cmd->last_fdout]);
	}
	else if (cmd->next)
	{
		printf("Pipe output: end[1] = %d\n", end[1]);
		if (dup2(end[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(end[1]);
	}

	// Close unnecessary file descriptors
	close_fds(cmd);
}

/* void	redirect_fds(t_cmd *cmd, int *end) */
/* { */
/*     if (!cmd->io_list) */
/*         return ; */
/*     if (cmd->infd[cmd->last_fdin]) */
/*         dup2(cmd->infd[cmd->last_fdin], STDIN_FILENO); */
/*     else if (cmd->prev) */
/*         dup2(end[0], STDIN_FILENO); */
/*     if (cmd->outfd[cmd->last_fdout]) */
/*         dup2(cmd->outfd[cmd->last_fdout], STDOUT_FILENO); */
/*     else if (cmd->next) */
/*         dup2(end[1], STDOUT_FILENO); */
/*     if (cmd->prev) */
/*         close(end[0]); */
/*     if (cmd->next) */
/*         close(end[1]); */
/*     close_fds(cmd);//close all opened infiles & outfiles */
/* } */

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
