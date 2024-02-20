/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 18:47:28 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:03:06 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

/*
1. Writes n zeroed bytes to the string s.
2. If n is zero, bzero() does nothing.
Notes: void pointer cannot be dereferenced.

why cast to char: to operate on a byte-by-byte basis.
To ensures the pointer points to a single byte of memory
(since char is guaranteed to be 1 byte in C).


*/
void	*ft_bzero(void *s, size_t n)
{
	void	*ret;

	ret = s;
	if (n == 0)
		return (ret);
	while (n--)
		*(unsigned char *)s++ = 0;
	return (ret);
}

/*int main(void)
{
	char str1[100] = "Hello, world!";
	char str2[100] = "Hello, world!";

	// Use memset to initialize str1
	memset(str1, 0, sizeof(str1));
	// Use ft_bzero to initialize str2
	ft_bzero(str2, sizeof(str2));

	// Compare the results
	if (memcmp(str1, str2, sizeof(str1)) == 0)
		printf("ft_bzero works correctly.\n");
	else
		printf("ft_bzero does not work correctly.\n");

	return 0;
}*/
