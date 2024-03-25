/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:31 by xiwang            #+#    #+#             */
/*   Updated: 2024/03/24 23:02:02 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

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

t_cmd	*creat_cmd(int size, t_data *data)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->s= NULL;
	new->is_builtin = 0;
	new->infile = NULL;
	new->outfile = NULL;
	new->hdfile = NULL;
	new->next = NULL;
	new->infd = STDIN_FILENO;
	new->outfd = STDOUT_FILENO;
	new->in_type = 0;
	new->o_type = 0;
	new->data = data;
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
	}
}

void	del_token(t_token **head, t_token *node)
{
	if (!head || !*head || !node)
		return ;
	//update the prev node's next
	if (node->prev)
		node->prev->next = node->next;
	else
		*head = node->next;
	//update the next node's prev
	if (node->next)
		node->next->prev = node->prev;
	free(node->value);
	free(node);
}

void print_cmd_list(t_cmd *cmd, t_data *data)
{
	while (cmd)
	{
		if (!cmd)
			return ;
		int k = 0;
		while (cmd)
		{
			printf("******cmd%d*****\n", k);
			int i = 0;
			while (cmd->s[i])
			{
				printf("%s ", cmd->s[i]);
				i++;
			}
			printf("\n");
			//printf("\ncmd_redir = %d\n", cmd->is_builtin);
			cmd = cmd->next;
			k++;
		}
	}
}
