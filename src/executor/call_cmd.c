/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 19:27:42 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/27 17:36:01 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_tilde_command(t_cmd *cmd)
{
	char	*expanded_arg;

	if (ft_strncmp(cmd->s[0], "~", 2) == 0)
	{
		expanded_arg = expand_tilde(cmd->s[0]);
		print_error("minishell: ", expanded_arg, ": Is a directory\n");
		free(expanded_arg);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	is_empty_command(t_cmd *cmd)
{
	if (ft_strncmp(cmd->s[0], "", 1) == 0 || if_all_space(cmd->s[0]))
	{
		print_error("minishell: ", cmd->s[0], ": command not found\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	wait_for_child(pid_t pid, t_data *data)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		free_exit("waitpid", data, EXIT_FAILURE);
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (EXIT_SIGINT);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_SUCCESS);
}

static void	execute_command(t_cmd *cmd, t_data *data)
{
	execve(cmd->s[0], cmd->s, data->env);
	free_exit("execve", data, EXIT_FAILURE);
}

int	call_cmd(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	int		exit_code;

	exit_code = 0;
	status = 0;
	if (!cmd->s || cmd->empty_var == 1)
		return (0);
	if (is_empty_command(cmd) != 0)
		return (EXIT_CMD_NOT_FOUND);
	if (is_tilde_command(cmd) != 0)
		return (EXIT_CMD_EXEC_ERROR);
	exit_code = find_executable_and_execute(cmd, data);
	if (exit_code != 0)
		return (exit_code);
	pid = fork();
	if (pid == 0)
		execute_command(cmd, data);
	else
		status = wait_for_child(pid, data);
	return (status);
}
