#include "../../includes/minishell.h"

void redirect_io(t_cmd *cmd) {
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

void setup_stdio_backups(t_cmd *cmd) {
	cmd->stdin_backup = dup(STDIN_FILENO);
	cmd->stdout_backup = dup(STDOUT_FILENO);
}

void reset_stdio(t_cmd *cmd) {
	if (cmd->stdout_backup != -1) {
		dup2(cmd->stdout_backup, STDOUT_FILENO);
		close(cmd->stdout_backup);
	}
	if (cmd->stdin_backup != -1) {
		dup2(cmd->stdin_backup, STDIN_FILENO);
		close(cmd->stdin_backup);
	}
}
