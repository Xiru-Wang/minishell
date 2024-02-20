/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:13:49 by xiruwang          #+#    #+#             */
/*   Updated: 2023/05/24 19:13:58 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

/*
returns a pointer to the first occurrence of the character c in the string s.
*/
char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == 0)
		return ((char *)s);
	return (0);
}

/*int main()
{
	char str[] = "Hello, world,";
	int c = ',';

	printf("Lib's:%s\n", strchr(str, c));
	printf("Mine's:%s\n", ft_strchr(str, c));
	return 0;
}*/
