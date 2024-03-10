/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:37:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/03/05 18:45:09 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	is_space(char c)
{
	if ( c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}


int	if_all_space(char *s)
{
	while (*s)
	{
		if (is_space(*s) == 0)
			return (0);
		s++;
	}
	return (1);
}

//use index to iterate so will not lose original pointer
void	free_double_ptr(char **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

// void	free_token_list(t_token **list)
// {
// 	t_token	*temp;

// 	if ((*list) == NULL || list == NULL)
// 		return ;
// 	while (*list)
// 	{
// 		temp = (*list)->next;
// 		if ((*list)->value)
// 			free ((*list)->value);
// 		free(*list);
// 		*list = temp;
// 	}
// 	*list = NULL;
// }

void	free_cmd_list(t_cmd **cmd)
{
	t_cmd	*temp;

	if ((*cmd) == NULL || cmd == NULL)
		return ;
	while (*cmd)
	{
		temp = (*cmd)->next;
		if ((*cmd)->s)
			free_double_ptr((*cmd)->s);
		free(*cmd);
		*cmd = temp;
	}
	*cmd = NULL;
}

void	free_exit(char *s, t_data *data, int code)
{
	if (s)
		printf("%s\n", s);
	free_token_list(&data->token_list);
	free(data->line);
	if (data->in)
		free(data->in);
	if (data->out)
		free(data->out);
	if (data->cmd_list)
		free_cmd_list(&data->cmd_list);
	free(data);
	exit(code);
}
