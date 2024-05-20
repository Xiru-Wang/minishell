/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:36:51 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 15:24:02 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_data(t_data *data)
{
	if (!data)
		return;
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	free_token_list(&data->token_list);
	free_cmd_list(&data->cmd_list);
	if (data->pwd)
	{
		free(data->pwd);
		data->pwd = NULL;
	}
	if (data->old_pwd)
	{
		free(data->old_pwd);
		data->old_pwd = NULL;
	}
	if (data->var_name)
	{
		free(data->var_name);
		data->var_name = NULL;
	}
	if (data->env)
	{
		free_double_ptr(data->env);
		data->env = NULL;
	}
	if (data)
	{
		free(data);
		data = NULL;
	}
}

void	free_cmd_list(t_cmd **cmd)
{
	t_cmd	*temp;

	if (cmd == NULL || *cmd == NULL)
		return;
	while (*cmd)
	{
		temp = (*cmd)->next;
		if ((*cmd)->s)
		{
			free_double_ptr((*cmd)->s);
			(*cmd)->s = NULL;
		}
		if ((*cmd)->io_list)
		{
			free_io_list(&(*cmd)->io_list);
			(*cmd)->io_list = NULL;
		}
		free(*cmd);
		*cmd = temp;
	}
	*cmd = NULL;
}

void	free_exit(char *s, t_data *data, int code)
{
	if (s)
		printf("%s\n", s);
	free_data(data);
	exit(code);
}

void	free_token_list(t_token **list)
{
	t_token *temp;
	if (list == NULL || *list == NULL)
		return;
	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value)
		{
			free((*list)->value);
			(*list)->value = NULL;
		}
		free(*list);
		*list = temp;
	}
	*list = NULL;
}

/* use index to iterate so will not lose original pointer */
void	free_double_ptr(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return;
	while (ptr[i] != NULL) 
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}
