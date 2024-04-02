/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 18:52:43 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/23 11:32:02 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

/*locates the first occurrence of c (converted to an unsigned char)
 in string s.
it returns a pointer to the byte located, or NULL if no such byte exists
within n bytes.
unsigned char: 8 bits, char: 1 sign bit + 7 bits
eg. -1 will become 255, instead of 128
*/

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n--)
	{
		if (*p == (unsigned char)c)
			return ((void *)p);
		p++;
	}
	return (NULL);
}

/*int main()
{
	char str[] = "Hello, world,";
	int c = '?';
	size_t n = 0;

	char *p1 = memchr(str, c, n);
	char *p2 = ft_memchr(str, c, n);
	printf("Lib's:%s\n", p1);
	printf("Mine's:%s\n", p2);
	return 0;
}*/
