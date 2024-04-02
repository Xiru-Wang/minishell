/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 18:40:57 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:07:12 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

/*compare two memory block
1. The memcmp() function compares byte string s1 against byte string s2.
Both strings are assumed to be n bytes long.
2. Returns the difference between the first two differing bytes
3. Comparing exactly n bytes, regardless of their content.
*/

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (*p1 == *p2 && --n)
	{
		p1++;
		p2++;
	}
	return (*p1 - *p2);
}

/*
int main()
{
    char str1[] = "Hello, world!";
    char str2[] = "HEllo, world!";

    printf("Mine:%d, Lib's:%d\n",\
     ft_memcmp(str1, str2, 1), memcmp(str1, str2, 1));
    return 0;
}
*/

/*
comparing exactly n bytes, regardless of their content.
memcmp 可以用来比较任意两个内存区域的值，而不仅限于字符串，而 strcmp 只能比较字符串。
*/
