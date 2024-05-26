/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:37:05 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/26 12:58:18 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_error_exit(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	return (2);
}

int	too_many_arguments_exit(char *arg)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	free(arg);
	return (EXIT_FAILURE);
}

int	empty_str_exit(char *s)
{
	if (if_all_space(s) == 1)
	{
		printf("exit\n");
		printf("minishell: exit:%s: numeric argument required\n", s);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
