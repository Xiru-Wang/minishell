/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:44:35 by xiruwang          #+#    #+#             */
/*   Updated: 2023/05/24 19:15:15 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (*s1 && (*s1 == *s2) && --n)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/*
int main()
{
	size_t n = 1;
	char str1[] = "Hello, world!";
	char str2[] = "HEllo, w";

	printf("Mine:%d, Lib's:%d\n",\
	 ft_strncmp(str1, str2, n), strncmp(str1, str2, n));
	return 0;
}
//首先考虑n为零的情况
//注意必须是--n， 否则n=1时会进入循环,s1,s2会移动到[1]位置，返回s1[1]-s2[1]
//因为N是无符号数，n=0时，--n= -1 时，n实际上被转换为了一个很大的正整数 无法跳出循环
*/
