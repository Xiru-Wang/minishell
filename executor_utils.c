/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:10:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/03/10 16:24:11 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	assign_cmd_id(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		cmd->id = i;
		cmd = cmd->next;
		i++;
	}
}
