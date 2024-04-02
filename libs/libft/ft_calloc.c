/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:04:56 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/25 16:03:39 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

/*
Have to #include <stdint.h>//for SIZE_MAX
malloc then filled with zeros.
*/
/*void	*ft_bzero(void *s, size_t n)
{
	void	*ret;

	ret = s;
	if (n == 0)
		return (ret);
	while (n--)
		*(unsigned char *)s++ = 0;
	return (ret);//return pair with void *func
}*/

void	*ft_calloc(size_t count, size_t size)
{
	void	*ret;

	if (count != 0 && SIZE_MAX / count < size)
		return (NULL);
	ret = malloc(count * size);
	if (ret == NULL)
		return (ret);
	ft_bzero(ret, size * count);
	return (ret);
}

/*int main(void)
{
	printf("%zu\n", SIZE_MAX);//to check the number in this system
    size_t size = 10;
    int *arr = (int *)ft_calloc(size, sizeof(int));

    for (size_t i = 0; i < size; i++)
        printf("%d ", arr[i]);
    free(arr);
    return 0;
}*/
