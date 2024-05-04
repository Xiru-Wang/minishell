/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:31 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/05 00:20:27 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*init_cmd(t_data *data)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
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

	size = 1;
	while (list && list->type != PIPE)
	{
		size++;
		list = list->next;
	}
	return (size);
}

void	print_cmd_list(t_cmd *cmd)
{
	while (cmd)
	{
		if (!cmd)
			return ;
		while (cmd)
		{
			int i = 0;
			while (cmd->s[i])
				i++;
			cmd = cmd->next;
		}
	}
}

