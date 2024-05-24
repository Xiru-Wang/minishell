/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:37:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/24 08:40:01 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_space(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_str_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	**ft_arrdup(char **arr)
{
	int		i;
	int		len;
	char	**new_arr;

	if (arr == NULL)
		return (NULL);
	len = 0;
	while (arr[len])
		len++;
	new_arr = (char **)malloc(sizeof(char *) * (len + 1));
	if (new_arr == NULL)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (new_arr[i] == NULL)
		{
			free_double_ptr(new_arr);
			return (NULL);
		}
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}

int	if_all_space(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (is_space(s[i]) == 1)
			i++;
		else
			break ;
	}
	if (i == ft_strlen(s))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	free_exit(char *s, t_data *data, int code)
{
	if (s)
		printf("%s\n", s);
	free_data(data);
	exit(code);
}
