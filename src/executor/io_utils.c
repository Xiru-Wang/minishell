/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:10:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/20 20:38:13 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_io	*init_io(t_cmd *cmd)
{
	t_io	*new;

	new = (t_io *)malloc(sizeof(t_io));
	if (!new)
		free_exit("malloc error", cmd->data, EXIT_FAILURE);
	new->filename = NULL;
	new->hdfile = NULL;
	new->next = NULL;
	new->eof = NULL;
	new->type = -1;
	return (new);
}

void	append_io(t_io **head, t_io *new)
{
	t_io	*temp;

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

void	free_io_list(t_io **list)
{
	t_io	*temp;

	temp = NULL;
	if (list == NULL || *list == NULL)
		return ;
	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->filename)
			free((*list)->filename);
		if ((*list)->eof)
			free((*list)->eof);
		if ((*list)->hdfile)
		{
			unlink((*list)->hdfile);
			free((*list)->hdfile);
		}
		free(*list);
		*list = temp;
	}
}
