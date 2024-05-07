#include "../../includes/minishell.h"

void redirect_io_simple(t_cmd *cmd) {
	t_io *io = cmd->io_list;
	int fd;

	fd = 0;
	while (io) 
	{
		if (io->type == REDIR_OUT || io->type == APPEND) 
		{
			// Handle output redirection and appending
			fd = open(io->filename, O_WRONLY | O_CREAT | (io->type == APPEND ? O_APPEND : O_TRUNC), 0666);
			if (fd == -1) 
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (io->type == REDIR_IN) 
		{
			// Handle input redirection
			fd = open(io->filename, O_RDONLY);
			if (fd == -1) 
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		io = io->next;
	}
}

void redirect_io(t_cmd *cmd, int *end) {
    // Redirect standard input if needed
    if (cmd->prev) {  // There is a command before this one
        dup2(end[0], STDIN_FILENO);
        close(end[0]);  // Close the read-end of the pipe after dup2
    }

    // Redirect standard output to the next part of the pipeline
    if (cmd->next) {  // There is a command after this one
        dup2(end[1], STDOUT_FILENO);
        close(end[1]);  // Close the write-end of the pipe after dup2
    }

    // Handle additional redirections
    redirect_io_simple(cmd);
}

void reset_stdio(t_cmd *cmd) {
    dup2(cmd->stdin_backup, STDIN_FILENO);
    dup2(cmd->stdout_backup, STDOUT_FILENO);
    close(cmd->stdin_backup);
    close(cmd->stdout_backup);
}
