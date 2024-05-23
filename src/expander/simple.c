/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:18:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/23 15:33:09 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*add_expand_value(char *s, char *dst, t_data *data, int *i)
{
	char	*value;
	char	*new;

	value = handle_dollar(s, i, data->env, data);
	new = ft_strjoin(dst, value);
	free(dst);
	free(value);
	return (new);
}

// in this function: vars expand without careing quotes
char	*expand_simple(char *s, t_data *data)
{
	int		i;
	char	*dst;
	char	*value;
	char	*temp;

	i = 0;
	dst = ft_calloc(1, sizeof(char));
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
			dst = add_expand_value(s + i, dst, data, &i);
		else
		{
			value = char_to_str(s[i]);
			temp = ft_strjoin(dst, value);
			free(dst);
			dst = temp;
			free(value);
			i++;
		}
	}
	return (dst);
}

// char	*expand_simple(char *s, char **env, t_data *data)
// {
// 	int		i;
// 	int		k;
// 	char	*dst;
// 	char	*value;
// 	size_t	dst_size;
// 	size_t	dst_len;

// 	dst_size = ft_strlen(s) + 1;
// 	dst = ft_calloc(dst_size, sizeof(char));
// 	dst_len = 0;
// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
// 		{
// 			k = 0;
// 			value = expand_dollar(s + i, &k, env, data);
// 			if (value)
// 			{
// 				size_t value_len = ft_strlen(value);
// 				if (dst_len + value_len + 1 > dst_size)
// 				{
// 					dst_size = dst_len + value_len + 1;
// 					char *new_dst = ft_calloc(dst_size, sizeof(char));
// 					ft_strlcpy(new_dst, dst, dst_len + 1);
// 					free(dst);
// 					dst = new_dst;
// 				}
// 				ft_strlcat(dst, value, dst_size);
// 				dst_len += value_len;
// 				free(value);
// 			}
// 			i = i + k;
// 		}
// 		else
// 		{
// 			if (dst_len + 2 > dst_size)
// 			{
// 				dst_size = dst_len + 2;
// 				char *new_dst = ft_calloc(dst_size, sizeof(char));
// 				ft_strlcpy(new_dst, dst, dst_len + 1);
// 				free(dst);
// 				dst = new_dst;
// 			}
// 			value = char_to_str(s[i]);
// 			ft_strlcat(dst, value, dst_size);
// 			dst_len += 1;
// 			free(value);
// 			i++;
// 		}
// 	}
// 	return (dst);
// }

// char	*expand_dollar(char *s, int *len, char **env, t_data *data)
// {
// 	int		var_len;
// 	char	*var_name;
// 	char	*value;

// 	value = NULL;
// 	if (*s == '$')
// 	{
// 		s++;
// 		if (*s == '?')
// 		{
// 			value = ft_itoa(data->exit_code);
// 			*len = 2;
// 		}
// 		else if (ft_isdigit(*s))
// 		{
// 			*len = 2;
// 			return (NULL);
// 		}
// 		else
// 		{
// 			var_len = count_var_len(s);
// 			var_name = ft_substr(s, 0, var_len);
// 			value = find_env(var_name, env);
// 			free(var_name);
// 			*len = 1 + var_len;
// 		}
// 	}
// 	return (value);
// }

//should check len within double quotes(include quotes)
int	len_within_quo(char *s, char c)
{
	int	len;

	len = 0;
	if (*s == c)
	{
		s++;
		while (*s && *s != c)
		{
			len++;
			s++;
		}
	}
	return (len + 2);
}
