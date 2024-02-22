#include "includes/minishell.h"

t_token	*create_token(char *s, t_type type, int n)
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
		temp = temp->next;//arrive at last node
	temp->next = new;//let last node point new
	new->prev = temp;//MISTAKE temp->prev = temp;
}

int	add_list(char *str, t_type type, t_token **head, int n)
{
	t_token	*new;

	new = create_token(str, type, n);
	if (!new)
		return (0);
	token_add_back(head, new);
	return (1);
}

void	print_list(t_token *token_list)
{
	while (token_list != NULL)
	{
		printf("value: %s, index: %d\n", token_list->value, token_list->i);
		token_list = token_list->next;
	}
}

void	free_node(t_token **list)
{
	t_token	*temp;

	if ((*list) == NULL)
		return ;
	while (*list)
	{
		temp = (*list)->next;
		free ((*list)->value);
		free(*list);
		*list = temp;
	}
	*list = NULL;
}
