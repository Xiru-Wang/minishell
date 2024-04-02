/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 18:56:02 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/25 15:57:49 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	k;

	i = 0;
	k = 0;
	while (dst[i] && i < dstsize)
		i++;
	while (src[k] && i + 1 < dstsize)
	{
		dst[i] = src[k];
		i++;
		k++;
	}
	if (i < dstsize)
		dst[i] = 0;
	while (src[k])
	{
		i++;
		k++;
	}
	return (i);
}

/* int main()
{
	char dst[10] = "A";
	char src[] = "1234567";
	char dst1[10] = "A";
	char src1[] = "1234567";

	size_t n = 10;
	printf("Lib's:%lu\n", strlcat(dst, src, n));
	printf("Lib's:%s\n", dst);
	printf("Mine:%lu\n", ft_strlcat(dst1, src1, n));
	printf("Mine:%s\n", dst1);

	return 0;
}
*/

/*
strlcat() returns the initial length of dst plus the length of src.
The strlcat() function appends the NUL-terminated string src to the end of dst.
It will append at most size - strlen(dst) - 1 bytes
这个函数实现的是将字符串src追加到dst的末尾，并返回整个字符串的长度（不包括末尾的空字符'\0'）。
函数会保证dst字符串不会超过给定的dstsize大小。

如果dstsize为0，则dst不会被修改，函数会直接返回src的长度。
因为dst的空间大小为0，无法将src复制到dst中，所以无需对dst进行任何更改。
*/
