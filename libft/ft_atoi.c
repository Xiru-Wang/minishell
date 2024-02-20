/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:05:12 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:31:52 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
NOTE: 'Normal' atoi NOT 42 ATOI. eg. ft_atoi("-+42") == 0
normal atoi cannot handle more than one '-'/'+' sign before the numbers
converts char number (a string) to int
*/

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

/*int main(void)
{
    char str[] = "+-42";
    printf("%d\n", ft_atoi(str));//0
    printf("%d\n", atoi(str));//0

    return 0;
}*/
