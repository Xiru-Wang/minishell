/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 15:05:58 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/25 16:00:42 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	num_len(int n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char			*ret;
	size_t			len;
	unsigned int	num;

	len = num_len(n);
	if (n < 0)
		num = -n;
	else
		num = n;
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (ret == NULL)
		return (NULL);
	ret[len] = 0;
	while (len > 0)
	{
		ret[len - 1] = num % 10 + '0';
		num = num / 10;
		len--;
	}
	if (n < 0)
		ret[0] = '-';
	return (ret);
}

/*int	main(void)
{
	int	a;
	int	b;
	int	c;

	a = -42;
	b = -2147483648;
	c = 2147483647;
	printf("%s\n", ft_itoa(a));
	printf("%s\n", ft_itoa(b));
	printf("%s\n", ft_itoa(c));
	return (0);
}*/
