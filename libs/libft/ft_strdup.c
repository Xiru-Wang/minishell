/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 18:54:44 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/15 19:11:27 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
1. malloc pace
2. strlcpy
3. return a pointer to the new string
If the memory allocation fails, strdup returns a null pointer.
*/

char	*ft_strdup(const char *s)
{
	char	*ret;
	size_t	len;

	len = ft_strlen(s) + 1;
	ret = (char *)malloc(len * sizeof(char));
	if (ret == NULL)
		return (NULL);
	ft_strlcpy(ret, s, len);
	return (ret);
}

/*
The main difference between strdup and strlcpy is that:
strdup:allocates new memory and returns a pointer to the new string,
strlcpy:copies a string into an existing buffer.
You should use strdup when you want to create a new copy of a string,
and strlcpy when you want to copy a string into a buffer safely.
*/
