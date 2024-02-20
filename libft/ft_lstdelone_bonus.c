/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 18:23:39 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:31:09 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
this function:
1. free the memory of the node’s content using the function ’del’
2. free the node.
Attention: The memory of ’next’ must not be freed.

Learn more <Single Linked List (Deleting the Node at a Particular Position)>
https://www.youtube.com/watch?v=f1r_jxCyOl0
*/

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL)
		return ;
	del(lst -> content);
	free(lst);
}

/*void	free_content(void *content)
{
	free(content);
}

int	main()
{

	t_list *node = malloc(sizeof(t_list));
	char *content = strdup("Hello, World!");
	node->content = content;
	node->next = NULL;
	// char arr[]= "Hello, World!";
	// node->content = arr;
	//NOT WORKING:character array arr is allocated on the stack,
	// you don't need to free it explicitly.

	//node->content = "hello world";
	// string literals are typically stored in read-only memory.
	//The free can only free dynamic memorry

	// Call ft_lstdelone to delete the node and its content
	ft_lstdelone(node, free_content);
	printf("Node and content freed successfully!\n");
	return 0;
}*/
