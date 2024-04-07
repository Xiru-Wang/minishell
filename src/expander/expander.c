/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:34:04 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/07 15:13:13 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expander(char *s, int len)
{
	char	*var_name;
	char	*value;

	value = NULL;
	var_name = ft_substr(s, 0, len);
	value = getenv(var_name);
	free(var_name);
	return (value);
}
