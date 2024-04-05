/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:37:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/04/05 18:50:23 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

// ERROR VERSION
/* void	free_cmd_list(t_cmd **cmd) */
/* { */
/*     t_cmd	*temp; */
/*  */
/*     if ((*cmd) == NULL || cmd == NULL) */
/*         return ; */
/*     while (*cmd) */
/*     { */
/*         temp = (*cmd)->next; */
/*         if ((*cmd)->s) */
/*             free_double_ptr((*cmd)->s); */
/*         free_io_list((*cmd)->io_list); */
/*         free(*cmd); */
/*         *cmd = temp; */
/*     } */
/*     *cmd = NULL; */
/* } */

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
