/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:31 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/15 23:17:59 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*init_cmd(t_data *data)
{
	t_cmd	*new;

	new = (t_cmd *)calloc(1, sizeof(t_cmd));//changed
	if (!new)
		return (NULL);
	new->s= NULL;
	new->is_builtin = 0;
	new->next = NULL;
	new->prev = NULL;
	new->io_list = NULL;
	new->delimiter = NULL;
	new->hdfile = NULL;
	new->data = data;
	new->infd = -1;
	new->outfd = -1;
	new->stdin_backup = -1;
	new->stdout_backup = -1;
	return (new);
}

void	append_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*temp;

	if (!head || !new)
		return ;
	if (*head == NULL)
		*head = new;
	else
	{
		temp = *head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
}

int	count_pipe(t_token *list)
{
	int	i;

	i = 0;
	while (list)
	{
		if (list->type == PIPE)
			i++;
		list = list->next;
	}
	return (i);
}

int	count_args(t_token *list)
{
	int	size;

	size = 0;//was : size = 1
	while (list && list->type != PIPE)
	{
		size++;
		list = list->next;
	}
	return (size);
}


void	print_cmd_list(t_cmd *cmd)
{
	if (!cmd)
			return ;
	while (cmd)
	{
		int k = 0;
		printf("******cmd%d*****\n", k);
		int i = 0;
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

