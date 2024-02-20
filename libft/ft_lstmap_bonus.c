/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 18:25:26 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:35:37 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
what is actually happening:
make changes on the copy of the list, if failed, free the copied list
in lstiter(), f()apply original list
1. Iterates the list
2. applies the function 'f' on the content
3. The 'del' function: delete the content of a node if needed.
*/

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*new_node;

	if (!lst || !f || !del)
		return (NULL);
	head = NULL;
	while (lst)
	{
		new_node = ft_lstnew (f(lst->content));
		if (!new_node)
		{
			ft_lstclear (&head, del);
			return (NULL);
		}
		ft_lstadd_back (&head, new_node);
		lst = lst->next;
	}
	return (head);
}
