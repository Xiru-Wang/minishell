/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:09:19 by xiwang            #+#    #+#             */
/*   Updated: 2023/05/24 19:30:16 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
apply a function to each character of a string
ft_striteri doesn't return anything and works directly
on the original string.

return statement in void serves to exit the function and
return to the location where the function was called(eg. int main).
*/

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (s == NULL || f == NULL)
		return ;
	while (s[i])
	{
		f (i, &s[i]);
		i++;
	}
}

/*
void	print_s(unsigned int i, char *s)
{
		printf("i = %d, s[%d] = %c\n", i, i, *s);
}
//DONT ITERATE AGAIN

int main()
{
	char s[] = "hi there";//char array, not char pointer
	ft_striteri(s, print_s);
	return 0;
}
*/
