/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:36:51 by jschroed          #+#    #+#             */
/*   Updated: 2024/04/20 12:36:53 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_arr(char **arr)
{
    int i;

    if (arr == NULL)
        return;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void free_data(t_data *data)
{
    free_arr(data->env);
    free(data->line);
    free_token_list(&data->token_list);
    free_cmd_list(&data->cmd_list);
    free(data->pwd);
    free(data->old_pwd);
}

void free_cmd_list(t_cmd **cmd)
{
    t_cmd *temp;
    // First, check if the cmd pointer itself is NULL
    if (cmd == NULL)
        return;
    // Then, check if the list pointed to by cmd is NULL
    if (*cmd == NULL)
        return;
    while (*cmd)
    {
        temp = (*cmd)->next; // Save the next node
        // Free the string array if it exists
        if ((*cmd)->s)
            free_double_ptr((*cmd)->s);
        // Correctly call free_io_list by passing the address of the io_list member
        if ((*cmd)->io_list)
            free_io_list(&(*cmd)->io_list);
        free(*cmd); // Free the current node
        *cmd = temp; // Move to the next node
    }
    // The list is now empty, so *cmd is already NULL due to the loop
}

void	free_exit(char *s, t_data *data, int code)
{
	if (s)
		printf("%s\n", s);
	free_token_list(&data->token_list);
	free(data->line);
	free_cmd_list(&data->cmd_list);
	free(data);
	exit(code);
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

/* use index to iterate so will not lose original pointer */
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
