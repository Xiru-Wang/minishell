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

/**
 * Redirects input for a command.
 *
 * This function redirects the input for a given command by either duplicating
 * the file descriptor associated with the input file or by using a pipe to read
 * input from the previous command.
 *
 * @param cmd A pointer to the command structure containing information about
 * the command
 * @param end An array of integers representing the file descriptors for the
 * pipe
 *
 * @return void
 *
 * @throws perror("dup2") if an error occurs while duplicating the file
 * descriptor
 * @throws exit(EXIT_FAILURE) if an error occurs while closing the file
 * descriptor
 */
static void redirect_input(t_cmd *cmd, int *end)
{
	if (cmd->last_fdin >= 0 && cmd->infd[cmd->last_fdin] != 0)
	{
		if (dup2(cmd->infd[cmd->last_fdin], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->infd[cmd->last_fdin]);
	}
	else if (cmd->prev)
	{
		if (dup2(end[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(end[0]);
	}
}

/**
 * Redirects the output of a command to a specified file descriptor or pipe.
 *
 * This function takes a command structure and an array of file descriptors as
 * input. It checks if the command has a specified output file descriptor and
 * redirects the output of the command to that file descriptor. If no output
 * file descriptor is specified, it redirects the output to a pipe.
 *
 * @param cmd A pointer to the command structure containing information about
 * the command's output file descriptors.
 * @param end An array of file descriptors representing the pipe endpoints.
 *
 * @return void
 *
 * @throws perror() error message if dup2() fails to duplicate the file
 * descriptor.
 */
static void redirect_output(t_cmd *cmd, int *end)
{
	if (cmd->last_fdout >= 0 && cmd->outfd[cmd->last_fdout] != 0)
	{
		if (dup2(cmd->outfd[cmd->last_fdout], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->outfd[cmd->last_fdout]);
	}
	else if (cmd->next)
	{
		if (dup2(end[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(end[1]);
	}
}

/**
 * Redirects file descriptors for input and output for a given command.
 *
 * This function takes a command structure pointer and an integer pointer as
 * arguments. It first redirects the input file descriptor for the command using
 * the redirect_input function. Then, it redirects the output file descriptor
 * for the command using the redirect_output function. Finally, it closes any
 * file descriptors that are no longer needed for the command using the
 * close_fds function.
 *
 * @param cmd A pointer to a command structure containing information about the
 * command to be executed.
 * @param end A pointer to an integer indicating the end of the command
 * execution.
 *
 * @return void
 */
void redirect_fds(t_cmd *cmd, int *end)
{
	redirect_input(cmd, end);
	redirect_output(cmd, end);
	close_fds(cmd);
}

/**
 * Closes file descriptors associated with a given command.
 *
 * This function takes a pointer to a command structure and closes all file
 * descriptors associated with it.
 *
 * @param cmd A pointer to a command structure containing file descriptors to be
 * closed.
 */
static void close_fds(t_cmd *cmd)
{
	int i;
	int k;

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

/**
 * Opens a file for reading and returns the file descriptor.
 *
 * This function takes a string representing the file path and attempts to open
 * the file for reading using the open() system call. If the file cannot be
 * opened, an error message is printed to stderr and 0 is returned.
 *
 * @param s A string representing the file path to be opened for reading
 * @return The file descriptor of the opened file, or 0 if the file cannot be
 * opened
 */
static int get_infd(char *s)
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

/**
 * @brief Get the file descriptor for output redirection
 *
 * This function takes a pointer to a t_io struct representing a redirection and
 * opens the specified file for writing. If the redirection type is APPEND, the
 * file is opened in append mode. If the redirection type is not APPEND, the
 * file is opened in truncate mode.
 *
 * @param redir A pointer to a t_io struct representing the output redirection
 * @return The file descriptor for the opened file, or 0 if an error occurs
 */
static int get_outfd(t_io *redir)
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
