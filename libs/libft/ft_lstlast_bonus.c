/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 18:22:32 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:23:58 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst -> next != NULL)
		lst = lst->next;
	return (lst);
}

/*int	main()
{
	t_list *head = NULL;
	t_list *temp, *node;
	t_list *node1 = malloc(sizeof(t_list));//RETURN SIZE OF THE STRUCT
	t_list *node2 = malloc(sizeof(t_list));
	t_list *node3 = malloc(sizeof(t_list));

	if (!node1 || !node2 ||!node3)
		return 1;
	node1 -> content = "hello";
	node2 -> content = "cat";
	node3 -> content = "and dog?";
	//string literal will be automatically deallocated
	//when the program terminates.

	head = node1;
	node1 -> next = node2;
	node2 -> next = node3;
	node3 -> next = NULL;


	printf("%s\n", (char *)ft_lstlast(head)->content);
	//remember type cast && access to its content

	node = head;
	temp = head;
	while (node)
	{
		// printf("%s\n", (char *)node->content);
		temp = node;
		node = node->next;
		free(temp);
	}
	return 0;
}
*/
