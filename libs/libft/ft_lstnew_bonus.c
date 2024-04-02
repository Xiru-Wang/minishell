/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:10:34 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:06:30 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
1. Allocates and returns a new node.
2. new node -> content = parameter 'content'
3. 'next' is initialized to NULL.
*/

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (new_node == NULL)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

/*int main(void)
{
	char *str = "Hello, world!";
	t_list *node = ft_lstnew(str);

	if (node != NULL)
	{
		printf("Content: %s\n", (char *)node->content);
		printf("Next: %p\n", node->next);
	}
	else
	{
		printf("Failed to create a new node.\n");
	}

	free(node);
	return 0;
}
*/
