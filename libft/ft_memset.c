/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:05:58 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/23 18:03:01 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n)
{
	void	*ret;

	ret = s;
	while (n--)
	{
		*(unsigned char *)s = (unsigned char)c;
		s++;
	}
	return (ret);
}
/*(unsigned char *)s: cast then dereference to get its value
*/
