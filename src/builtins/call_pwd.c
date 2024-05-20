/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:36:58 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 20:14:56 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_pwd(t_cmd *cmd)
{
	ft_putendl_fd(cmd->data->pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
