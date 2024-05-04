/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:03:17 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/04 21:29:04 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_env(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->data->env[i])
	{
		ft_putendl_fd(cmd->data->env[i], STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
