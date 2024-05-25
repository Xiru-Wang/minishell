/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 19:02:11 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/25 19:04:54 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_token_list(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->value)
			printf("value: %s, type: %d\n", \
					token_list->value, token_list->type);
		else
			printf("value: (null)\n");
		token_list = token_list->next;
	}
}

void	print_cmd_list(t_cmd *cmd)
{
	int	k;
	int	i;

	if (!cmd)
		return ;
	while (cmd)
	{
		k = 0;
		printf("******cmd%d*****\n", k);
		i = 0;
		while (cmd->s[i])
		{
			printf("%s ", cmd->s[i]);
			i++;
		}
		printf("\n");
		cmd = cmd->next;
		k++;
	}
}

void	print_io_list(t_cmd *cmd)
{
	if (!cmd->io_list)
		return ;
	if (cmd->io_list)
	{
		while (cmd->io_list)
		{
			printf("io_list: %s\n", cmd->io_list->filename);
			printf("io_list type: %d\n", cmd->io_list->type);
			cmd->io_list = cmd->io_list->next;
		}
	}
}
