/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:19:05 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 19:35:04 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*char_to_str(char c)
{
	char	*temp;

	temp = ft_calloc(sizeof(char), 2);
	temp[0] = c;
	return (temp);
}

//check var_name after $
int	char_is_valid(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '?')
		return (1);
	else
		return (0);
}

char	*trim_extra_space(char *s1)
{
	char	*temp;
	char	*res;

	temp = ft_strtrim(s1, " ");
	res = ft_strjoin(temp, " ");
	free(temp);
	return (res);
}

char	*find_env(char *s, char **env)
{
	int		i;
	int		n;
	char	*res;
	char	*temp;

	i = 0;
	n = ft_strlen(s);
	res = NULL;
	while (env[i])
	{
		if (ft_strncmp(s, env[i], n) == 0 && env[i][n] == '=')
		{
			temp = ft_substr(env[i], n + 1, ft_strlen(env[i]) - n);
			if (temp[ft_strlen(temp) - 2] == ' ')
			{
				res = trim_extra_space(temp);
				free(temp);
				return (res);
			}
			else
				return (temp);
		}
		i++;
	}
	return (res);
}
