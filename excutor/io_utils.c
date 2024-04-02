/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:10:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/04/02 18:39:47 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_io	*init_io(void)
{
	t_io	*new;

	new = (t_io *)malloc(sizeof(t_io));
	if (!new)
		return (NULL);
	new->filename = NULL;
	new->next = NULL;
	new->type = -1;
	return (new);
}

static void	append_io(t_io **head, t_io *new)
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

int add_io_list(t_io **head)
{
    t_io *new; // Corrected type
    new = init_io(); // Assuming init_io returns a pointer to t_io
    if (!new)
        return (0);
    append_io(head, new); // Assuming append_io expects t_io ** and t_io *
    return (1);
}

// ERROR VERSION
/* int	add_io_list(t_io **head) */
/* { */
/*     t_token	*new; */
/*  */
/*     new = init_io(); */
/*     if (!new) */
/*         return (0); */
/*     append_io(head, new); */
/*     return (1); */
/* } */

void free_io_list(t_io **list)
{
    t_io *temp;
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
    // The list is now empty, so *list is already NULL due to the loop
}

// ERROR VERSION
/* void	free_io_list(t_io **list) */
/* { */
/*     t_token	*temp; */
/*  */
/*     if ((*list) == NULL || list == NULL) */
/*         return ; */
/*     while (*list) */
/*     { */
/*         temp = (*list)->next; */
/*         if ((*list)->filename) */
/*             free ((*list)->filename); */
/*         free(*list); */
/*         *list = temp; */
/*     } */
/*     *list = NULL; */
/* } */
