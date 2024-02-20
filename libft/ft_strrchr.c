/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 18:57:04 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:16:17 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	if (c == 0)
		return ((char *)s + i);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i--;
	}
	return (0);
}

/*int main()
{
	char str[] = "Hello";
	int c = '.';

	printf("Lib's: %s\n", strrchr(str, c));
	printf("Mine's: %s\n", ft_strrchr(str, c));
	return 0;
}

//must be int, NO unsigned/size_t, it will convert to max number
//=0, index[0]
// return ‘\0'
*/
