#include "includes/minishell.h"

t_token	*create_token(char *s, int type, int n)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = s;
	token->type = type;
	token->i = n;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

void	token_add_back(t_token **head, t_token *new)
{
	t_token	*temp;

	temp = *head;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
}

int	add_list(char *str, int type, t_token **head, int n)
{
	t_token	*new;

	new = create_token(str, type, n);
	if (!new)
		return (0);
	token_add_back(head, new);
	return (1);
}
//这意味着循环会继续执行直到stack为NULL。
//这种方式确保了链表中的每个节点都会被访问和打印，包括链表的最后一个节点

// void	print_list(t_token *token_list)
// {
// 	while (token_list)
// 	{
// 		if (token_list->value != NULL)
// 			printf("value: %s, index: %d\n", token_list->value, token_list->i);
// 		token_list = token_list->next;
// 	}
// }

void print_list(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->value)
			printf("value: %s, index: %d\n", token_list->value, token_list->i);
		else
			printf("value: (null), index: %d\n", token_list->i);
		token_list = token_list->next;
	}
}


void	free_token_list(t_token **list)
{
	t_token	*temp;

	if ((*list) == NULL)
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
