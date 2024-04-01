/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:58 by xiruwang          #+#    #+#             */
/*   Updated: 2024/04/01 19:29:10 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*create_token(char *s, int type, int n)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (*s)
		token->value = s;
	else
		token->value = NULL;
	token->type = type;
	token->i = n;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

static void	token_add_back(t_token **head, t_token *new)
{
	t_token	*temp;

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

int	add_token_list(char *s, int type, t_token **head, int n)
{
	t_token	*new;

	new = create_token(s, type, n);
	if (!new)
		return (0);
	token_add_back(head, new);

	return (1);
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

void print_token_list(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->value)
			printf("value: %s, index: %d, type: %d\n", token_list->value, token_list->i, token_list->type);
		else
			printf("value: (null), index: %d\n", token_list->i);
		token_list = token_list->next;
	}
}

void	free_token_list(t_token **list)
{
	t_token	*temp;

	if ((*list) == NULL || list == NULL)
		return ;
	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value)
			free ((*list)->value);
		free(*list);
		*list = temp;
	}
	*list = NULL;
}
