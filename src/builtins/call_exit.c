/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:37:05 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/16 21:34:57 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Determine the exit code based on the input string and data.
 *
 * This function takes in a string array and a data structure, and determines
 * the exit code based on the input string. If the second element of the string
 * array is not present, the exit code is set to 0. If the second element is a
 * valid digit, it is converted to an integer and used as the exit code. If the
 * second element is not a valid digit, an error message is printed to stderr
 * and the exit code is set to 255.
 *
 * @param str The input string array.
 * @param data The data structure (not used in this function).
 * @return The determined exit code.
 */
/* static int determine_exit_code(char **str) */
/* { */
/*     int exit_code; */
/*  */
/*     if (!str[1]) */
/*         exit_code = 0; */
/*     else */
/*     { */
/*         if (is_str_digit(str[1])) */
/*             exit_code = ft_atoi(str[1]); */
/*         else */
/*         { */
/*             ft_putstr_fd("minishell: exit: ", STDERR_FILENO); */
/*             ft_putstr_fd(str[1], STDERR_FILENO); */
/*             ft_putstr_fd(": numeric argument required\n", STDERR_FILENO); */
/*             exit_code = 255; */
/*         } */
/*     } */
/*     return exit_code; */
/* } */

/**
 * Exits the minishell program with a specified exit code.
 *
 * This function takes in a command structure `cmd` and a data structure `data`,
 * and exits the minishell program with a specified exit code. It first prints
 * "exit" to the standard error output, and then checks if there are too many
 * arguments provided in the command structure `cmd`. If there are too many
 * arguments, it prints an error message to the standard error output and
 * returns `EXIT_FAILURE`.
 *
 * @param cmd A pointer to the command structure containing the arguments for
 * the exit command.
 * @param data A pointer to the data structure containing the data for the
 * minishell program.
 * @return The exit code to be used to exit the minishell program.
 */
int call_exit(t_cmd *cmd, t_data *data)
{
	(void) data;

	if (cmd->s[1])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd("exit", STDERR_FILENO);
	free_data(data);
	exit(EXIT_SUCCESS);
}
