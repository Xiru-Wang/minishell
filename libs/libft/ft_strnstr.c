/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 18:56:43 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:29:39 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

/*
If little is an empty string, big is returned;
if little occurs nowhere in big, NULL is returned;
otherwise a pointer to the first character of the first
occurrence of little is returned.
*/

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t			i;
	size_t			k;
	unsigned char	*ret;

	i = 0;
	if (*s2 == 0)
		return ((char *)s1);
	ret = (unsigned char *)s1;
	while (ret[i] && i < len)
	{
		k = 0;
		if (ret[i] == s2[k])
		{
			while (ret[i + k] == s2[k] && i + k < len)
			{
				k++;
				if (s2[k] == 0)
					return (&((char *)s1)[i]);
			}
		}
		i++;
	}
	return (NULL);
}

/*int main()
{
	char str1[] = "aaabcabcd";
	char str2[] = "aaabc";

	printf("Lib's:  %s\n", strnstr(str1, str2, 5));
	printf("Mine's: %s\n", ft_strnstr(str1, str2, 5));
	return 0;
}*/
