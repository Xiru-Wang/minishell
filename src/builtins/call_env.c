/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:03:17 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/19 18:56:39 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_env(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->s[1])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	while (cmd->data->env[i])
	{
		ft_putendl_fd(cmd->data->env[i], STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
