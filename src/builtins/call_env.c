/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:03:17 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/03 20:26:51 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_env(t_cmd *cmd)
{
	int	i;

	printf("Debug: Displaying environment variables\n");
	printf("Debug: cmd->data->env address: %p\n", (void *)cmd->data->env);

	i = 0;
	while (cmd->data->env[i])
	{
		printf("Debug: env[%d]: %s\n", i, cmd->data->env[i]);
		ft_putendl_fd(cmd->data->env[i], STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
