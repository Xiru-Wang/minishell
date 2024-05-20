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
	if (data->exit_code == EXIT_SIGINT)
		return (EXIT_SIGINT);
	else
		return (0);
}

static int	execute_single_command(t_cmd *cmd)
{
    int status;

    if (check_hd(cmd) == EXIT_SIGINT)
        return (EXIT_SIGINT);
    backup_stdio(cmd);
    redirect_io(cmd);
	if (cmd->err)  // Check for redirection error
    {
        reset_stdio(cmd);
        return (1);  // Return an error status
    }
    if (cmd->is_builtin)
        status = call_builtin(cmd);
    else
        status = call_cmd(cmd->data, cmd);
    reset_stdio(cmd);
    return (status);
}

// in parent
// close(fd_in);// Close the previous read end
// fd_in = end[0];// Use the read end of the current pipe in the next iteration
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
		if (check_hd(current) == EXIT_SIGINT)
			return (EXIT_SIGINT);
		cmd->data->pid[i] = fork();
		if (cmd->data->pid[i] == 0)
			setup_child_process(current, end, fd_in);
		else if (cmd->data->pid[i] < 0)
        {
            perror("fork");
            return 1;
        }
		if (fd_in != STDIN_FILENO)
            close(fd_in);
		if (current->next)
		{
			close(end[1]);
			fd_in = end[0];
		}
		current = current->next;
		i++;
	}
	reset_stdio(cmd);
	return (wait_for_processes(cmd->data->pid, cmd->data->cmd_num));
}

//if there's pipe, child get data from parent's end[read]: dup2(end[0], STDIN)
//if cmd->next, close child's end[read], and redirect stdout to child's end[write]
//if io_redirection, handle additional redirections, dup2 again: dup2(infile/outfile)

static int setup_child_process(t_cmd *cmd, int *end, int fd_in)
{
    if (fd_in != STDIN_FILENO)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (cmd->next)
    {
        close(end[0]);
        dup2(end[1], STDOUT_FILENO);
        close(end[1]);
    }
    // Redirect I/O and handle errors
    redirect_io(cmd);
    if (cmd->err)
        exit(1);  // Exit with error code if redirection fails
    if (cmd->is_builtin)
        exit(call_builtin(cmd));
    else
        exit(call_cmd(cmd->data, cmd));
}

// static int	setup_child_process(t_cmd *cmd, int *end, int fd_in)
// {
// 	if (fd_in != 0)
// 	{
// 		dup2(fd_in, STDIN_FILENO);
// 		close(fd_in);
// 	}
// 	if (cmd->next)
// 	{
// 		close(end[0]);
// 		dup2(end[1], STDOUT_FILENO);
// 		close(end[1]);
// 	}
// 	redirect_io(cmd);
// 	if (cmd->is_builtin)
// 		exit (call_builtin(cmd));
// 	else
// 		exit (call_cmd(cmd->data, cmd));
// }

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
