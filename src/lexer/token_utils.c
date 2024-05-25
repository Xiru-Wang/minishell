/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:58 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/25 19:04:46 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*create_token(char *s, int type, t_data *data)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		free_exit("malloc error", data, STDERR_FILENO);
	if (*s)
		token->value = s;
	else
		token->value = NULL;
	token->type = type;
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

int	add_token_list(char *s, int type, t_token **head, t_data *data)
{
	t_token	*new;

	new = create_token(s, type, data);
	token_add_back(head, new);
	if (type == APPEND || type == HEREDOC)
		return (2);
	else
		return (1);
}

void	del_token(t_token **head, t_token *node)
{
	if (!head || !*head || !node)
		return ;
	if (node->prev)
		node->prev->next = node->next;
	else
		*head = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (node->value)
		free(node->value);
	free(node);
}
