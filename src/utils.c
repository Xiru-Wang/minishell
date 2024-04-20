/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:37:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/04/20 12:34:39 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_space(char c)
{
	if ( c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

/**
 * Check if a string is composed entirely of digits.
 *
 * This function takes a string as input and checks if all characters in the
 * string are digits.
 *
 * @param str The string to be checked
 * @return 1 if the string is composed entirely of digits, 0 otherwise
 */
int is_str_digit(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

/**
 * Duplicate an array of strings.
 *
 * This function takes an array of strings as input and duplicates it, returning
 * a new array of strings.
 *
 * @param arr The array of strings to duplicate
 * @return A new array of strings that is a duplicate of the input array
 */
char **ft_arrdup(char **arr)
{
    int i;
    int len;
    char **new_arr;

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
            free_arr(new_arr);
            return (NULL);
        }
        i++;
    }
    new_arr[i] = NULL;
    return (new_arr);
}

int	if_all_space(char *s)
{
	while (*s)
	{
		if (is_space(*s) == 0)
			return (0);
		s++;
	}
	return (1);
}
