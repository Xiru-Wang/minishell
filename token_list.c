#include "includes/minishell.h"

t_token	*create_token(char *s, t_type type)
{
	t_token	*token;
	//int		index;

	//index = 0;
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = s;
	token->type = type;
	//token->i = index++; //还是n++吧
	token->prev = NULL;
	token->next = NULL;

	return (token);
}

void token_add_back(t_token **head, t_token *new)
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

int	add_list(char *str, t_type type, t_token **head)
{
	t_token	*add;

	add = create_token(str, type);
	if (!add)
		return (0);
	token_add_back(head, add);
	return (1);
}

void print_list()
{

}

void free_node()
{

}
