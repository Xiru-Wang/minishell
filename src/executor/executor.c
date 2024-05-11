#include "../../includes/minishell.h"

static int execute_single_command(t_cmd *cmd);
static int execute_command_pipeline(t_cmd *cmd);
static int setup_child_process(t_cmd *cmd, int *end, int fd_in);
static int wait_for_processes(int *pids, int num_pids);

int    executor(t_cmd *cmd, t_data *data)
{
    data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
    if (cmd->next == NULL)
        data->exit_code = execute_single_command(cmd);
    else
        data->exit_code = execute_command_pipeline(cmd);
    free(data->pid);

    if (data->exit_code == 1)  // 检查exit_code是否为1,表示heredoc被中断
        return (1);
    else
        return (0);
}

// void	executor(t_cmd *cmd, t_data *data)
// {
// 	data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
// 	if (cmd->next == NULL)
// 		data->exit_code = execute_single_command(cmd);
// 	else
// 		data->exit_code = execute_command_pipeline(cmd);
// 	free(data->pid);
// }

static int execute_single_command(t_cmd *cmd)
{
	int	exit_status;

	setup_stdio_backups(cmd);
	check_hd(cmd);
	redirect_io(cmd);
	if (cmd->is_builtin)
		exit_status = call_builtin(cmd);
	else
		exit_status = call_cmd(cmd->data, cmd);
	reset_stdio(cmd);
	return (exit_status);
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
	while (current)
	{
		if (current->next)
			pipe(end);
		cmd->data->pid[i] = fork();
		if (cmd->data->pid[i] == 0)
		{
			setup_child_process(current, end, fd_in);
			//data->exit_code += setup_child_process(current, end, fd_in);
			//exit(EXIT_FAILURE);  // Should never be reached
		}
		if (current->next)
		{ // Parent Process
			close(end[1]);  // Close the write end of the pipe in the parent
			if (fd_in != STDIN_FILENO)
				close(fd_in);  // Close the previous read end
			fd_in = end[0];  // Use the read end of the current pipe in the next iteration
		}
		current = current->next;
		i++;
	}
	return (wait_for_processes(cmd->data->pid, cmd->data->cmd_num));
}

//被 dup2 覆盖的文件描述符会被自动关闭,你不需要手动关闭它们。
static int	setup_child_process(t_cmd *cmd, int *end, int fd_in)
{
	check_hd(cmd);
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


static int	wait_for_processes(int *pids, int num_pids)
{
	int	i;
	int	status;
	int	exit_status;
	int	current_status;

	i = 0;
	exit_status = 0;
	while (i < num_pids)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			free_exit("waitpid", NULL, EXIT_FAILURE);
		else if (WIFEXITED(status))
		{
			current_status = WEXITSTATUS(status);
			// Update the global exit status if it's the last process or if an error occurred
			if (i == num_pids - 1 || current_status != 0)
				exit_status += current_status;
		}
		i++;
	}
	return (exit_status);
}
