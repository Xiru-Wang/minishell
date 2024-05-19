/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:17 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/19 19:06:05 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//echo
//with -n: no new line
//without -n: new line
//s[1] has to be -n (with a random number of n's) eg. -nnnn->works
//eg> echo -nn -nnnn2 -n "hi " "dad" "blala    lala"
//-nnnn2 -n hi  dad blala    lala%


static void print_arguments(t_cmd *cmd, int start)
{
	while (cmd->s[start])
	{
		if (ft_strncmp(cmd->s[start], "$?", 3) == 0)
			ft_putnbr_fd(cmd->data->exit_code, STDOUT_FILENO);
		else
			ft_putstr_fd(cmd->s[start], STDOUT_FILENO);
		if (cmd->s[start + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		start++;
	}
}

/**
 * Check if the given argument is a valid "-n" option.
 *
 * This function checks if the argument starts with "-n" followed by only 'n'
 * characters.
 *
 * @param arg The argument to check
 * @return 1 if the argument is a valid "-n" option, 0 otherwise
 */
static int is_valid_n_option(char *arg)
{
	int i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[++i])
		if (arg[i] != 'n')
			return (0);
	return (1);
}

/**
 * @brief This function calls the echo command with the given arguments.
 *
 * This function takes a pointer to a t_cmd struct as input, which contains the
 * command and its arguments. It iterates through the arguments starting from
 * index 1, checking if each argument is a valid option for the echo command. If
 * a valid option is found, it sets a flag to indicate that at least one valid
 * option was present. It then calls the print_arguments function to print the
 * arguments starting from the current index. If no valid options were found, it
 * prints a newline character.
 *
 * @param cmd A pointer to a t_cmd struct containing the command and its
 * arguments
 *
 * @return void
 */
int	call_echo(t_cmd *cmd)
{
	int i;
	int flag;

	i = 1;
	flag = 0;
	while (cmd->s[i] && is_valid_n_option(cmd->s[i]))
	{
		flag = 1;
		i++;
	}
	print_arguments(cmd, i);
	if (flag == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
