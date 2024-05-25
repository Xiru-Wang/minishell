/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:31 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/25 16:41:14 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*init_cmd(t_data *data)
{
	t_cmd	*new;

	new = (t_cmd *)calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->s = NULL;
	new->is_builtin = 0;
	new->next = NULL;
	new->prev = NULL;
	new->io_list = NULL;
	new->data = data;
	new->infd = -1;
	new->outfd = -1;
	new->stdin_backup = -1;
	new->stdout_backup = -1;
	new->err = 0;
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

	size = 0;
	while (list && list->type != PIPE)
	{
		size++;
		list = list->next;
	}
	return (size);
}

int	check_syntax(t_token *next, t_data *data)
{
	if (!next || !next->value)
	{
		data->exit_code = 2;
		printf("minishell: syntax error near "
			"unexpected token `newline\'\n");
		return (EXIT_FAILURE);
	}
	if (next->type != STR)
	{
		data->exit_code = 2;
		printf("minishell: syntax error near "
			"unexpected token `%s\'\n", next->value);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// int	check_syntax(t_token *next)
// {
// 	if (!next || !next->value)
// 	{
// 		printf(SYNTAXERR);
// 		return (EXIT_FAILURE);
// 	}
// 	if (next->type != WORD && next->type != QUO)
// 	{
// 		printf("minishell: syntax error near"
// 			"unexpected token `%s\'\n", next->value);
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }
