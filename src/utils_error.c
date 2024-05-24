/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:37:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/24 20:17:58 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_error(const char *prefix, const char *arg, const char *suffix)
{
	char	*error_msg;
	size_t	len;

	len = ft_strlen(prefix) + ft_strlen(arg) + ft_strlen(suffix) + 1;
	error_msg = malloc(len);
	if (error_msg == NULL)
		return ;
	ft_strlcpy(error_msg, prefix, len);
	ft_strlcat(error_msg, arg, len);
	ft_strlcat(error_msg, suffix, len);
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	free(error_msg);
}
