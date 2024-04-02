/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:01:23 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/25 15:58:40 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

static size_t	ft_char_in_set(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*ret;

	start = 0;
	end = ft_strlen(s1);
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && ft_char_in_set(s1[start], set))
		start++;
	while (start < end && ft_char_in_set(s1[end - 1], set))
		end--;
	ret = (char *)malloc(sizeof(char) * (end - start + 1));
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (start < end)
		ret[i++] = s1[start++];
	ret[i] = 0;
	return (ret);
}

/*int	main(void)
{
	char	str1[] = "abcdefgcc";
	char	str2[] = "acbe";
	//char	str3[] = "ac";
	char	*trimmed;
	// char	str4[] = "de";

	trimmed = ft_strtrim(str1, str2);
	printf("%s\n", trimmed);
	free(trimmed);
	//printf("%s\n", ft_strtrim(str1, str3));
	//printf("%s\n", ft_strtrim(str1, str4));

	return (0);
}*/
