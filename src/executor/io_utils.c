/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:10:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/05 00:10:08 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_io	*init_io(t_data *data)
{
	t_io	*new;

	new = (t_io *)malloc(sizeof(t_io));
	if (!new)
		free_exit("malloc error", data, EXIT_FAILURE);
	new->filename = NULL;
	new->next = NULL;
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
    // Check if the list pointer itself is NULL or points to NULL
    if (list == NULL || *list == NULL)
        return;
    while (*list)
    {
        temp = (*list)->next; // Save the next node
        if ((*list)->filename)
            free((*list)->filename); // Free the filename if it exists
        free(*list); // Free the current node
        *list = temp; // Move to the next node
    }
}
