/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:01 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/02 19:12:51 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

enum s_builtin	ft_bubiltin(char *s)
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

void	call_builtin(t_cmd *cmd)
{
	enum s_builtin	i;

	i = cmd->is_builtin;
	if (i == CD)
		call_cd();
	else if (i == ECHO)
		call_echo(cmd);
	else if (i == PWD)
		call_pwd();
	else if (i == EXPORT)
		call_export();
	else if (i == UNSET)
		call_unset();
	else if (i == ENV)
		call_env(cmd);
	else if (i == EXIT)
		call_exit();
}
