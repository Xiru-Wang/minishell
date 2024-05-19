#include "../../includes/minishell.h"

static int	execute_single_command(t_cmd *cmd);
static int	execute_command_pipeline(t_cmd *cmd);
static int	setup_child_process(t_cmd *cmd, int *end, int fd_in);
static int	wait_for_processes(int *pids, int num_pids);

int	executor(t_cmd *cmd, t_data *data)
{
	data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
	if (cmd->next == NULL)
		data->exit_code = execute_single_command(cmd);
	else
		data->exit_code = execute_command_pipeline(cmd);
	if (data->pid)
		free(data->pid);
	if (data->exit_code == 130)
		return (1);
	else
		return (0);
}

static int execute_single_command(t_cmd *cmd)
{
    int status;

    if (check_hd(cmd) == 130)
        return (130);
    backup_stdio(cmd);
    redirect_io(cmd);
    if (cmd->is_builtin)
        status = call_builtin(cmd);
    else
        status = call_cmd(cmd->data, cmd);
    reset_stdio(cmd);
    return (status);
}

static int	execute_command_pipeline(t_cmd *cmd)
{
	int		end[2];
	int		fd_in;
	int		i;
	t_cmd	*current;

	fd_in = STDIN_FILENO;
	i = 0;
	current = cmd;
	backup_stdio(cmd);
	while (current)
	{
		if (current->next)
			pipe(end);
		if (check_hd(current) == 130)
			return (130);
		cmd->data->pid[i] = fork();
		if (cmd->data->pid[i] == 0)
			setup_child_process(current, end, fd_in);
		close(end[1]);
		if (current->next)
		{
			if (fd_in != STDIN_FILENO)
				close(fd_in);// Close the previous read end
			fd_in = end[0];// Use the read end of the current pipe in the next iteration
		}
		current = current->next;
		i++;
	}
	reset_stdio(cmd);
	return (wait_for_processes(cmd->data->pid, cmd->data->cmd_num));
}

//if there's pipe, get data from pipe(end[0]): dup2(end[0], STDIN)
//if cmd->next, close
//if new io_redirection, dup2 again: dup2(file, STDIN)
static int	setup_child_process(t_cmd *cmd, int *end, int fd_in)
{
	if (fd_in != 0)
	{
		dup2(fd_in, STDIN_FILENO);  // Redirect stdin for the current command
		close(fd_in);
	}
	if (cmd->next)
	{
		close(end[0]);// Close the read end of the pipe in the child
		dup2(end[1], STDOUT_FILENO);  // Redirect stdout to the pipe
		close(end[1]);
	}
	redirect_io(cmd);  // Handle additional redirections
	if (cmd->is_builtin)
		exit (call_builtin(cmd));  // Execute builtin and exit child process
	else
		exit (call_cmd(cmd->data, cmd));  // Execute external command and exit
}

//echo $? ---->> equal last child exit status🤔
static int    wait_for_processes(int *pids, int num_pids)
{
    int    i;
    int    status;
    int    exit_status;

    i = 0;
    exit_status = 0;
    while (i < num_pids)
    {
        waitpid(pids[i], &status, 0);
		i++;
	}
    if (WIFEXITED(status))
        exit_status = WEXITSTATUS(status);
    return (exit_status);
}
