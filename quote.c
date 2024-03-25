/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:59 by xiwang            #+#    #+#             */
/*   Updated: 2024/03/25 18:54:36 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// WRONG DID NOT consider "hi""hi"->hihi
//  IN THE LOOP, only if s[0] == quote
// eg."hi"" "'hi'
int check_unclosed_quotes(char *s, int quote, t_token **head, int n)
{
	int	i;
	int	count;
	char	c;

	i = 0;
	count = 1;
	c = s[0];
	while (s[i] && s[i]!= 32) // before space: new string
	{
		if (s[i] == c)
			count = 2;
		i++;
		c = s[i];//reset c to find next pair of quote
		count = 1;
	}
	if (count = 1)
		return (-1);
	else
		add_list(ft_substr(s, 0, i), quote, head, n);
	return (i); // Return the position after the closing quote
}
