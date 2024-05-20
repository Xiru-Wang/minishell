/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:01 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/20 15:36:02 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

enum s_builtin	ft_builtin(char *s)
{
	if (ft_strncmp(s, "cd", 3) == 0)
		return (CD);
	else if (ft_strncmp(s, "echo", 5) == 0)
		return (ECHO);
	else if (ft_strncmp(s, "pwd", 4) == 0)
		return (PWD);
	else if (ft_strncmp(s, "export", 7) == 0)
		return (EXPORT);
	else if (ft_strncmp(s, "unset", 6) == 0)
		return (UNSET);
	else if (ft_strncmp(s, "env", 4) == 0)
		return (ENV);
	else if (ft_strncmp(s, "exit", 5) == 0)
		return (EXIT);
	return (0);
}

int	call_builtin(t_cmd *cmd)
{
	enum s_builtin	i;

	i = cmd->is_builtin;
	if (i == CD)
		return (call_cd(cmd->data, cmd));
	else if (i == ECHO)
		return (call_echo(cmd));
	else if (i == PWD)
		return (call_pwd(cmd));
	else if (i == EXPORT)
		return (call_export(cmd, cmd->data));
	else if (i == UNSET)
		return (call_unset(cmd, cmd->data));
	else if (i == ENV)
		return (call_env(cmd));
	else if (i == EXIT)
		return (call_exit(cmd, cmd->data));
	return (EXIT_FAILURE);
}
