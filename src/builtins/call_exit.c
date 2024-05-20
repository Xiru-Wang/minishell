/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:37:05 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 16:02:16 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int call_exit(t_cmd *cmd, t_data *data)
{
	int last_exit_code;
	char *arg;
	int i;
	int sign;
	int invalid;
	unsigned long long num;
	unsigned long long prev_num;

	last_exit_code = data->exit_code;
	if (cmd->s[1])
	{
		arg = ft_strtrim(cmd->s[1], " \t");
		if (ft_strlen(arg) == 0 || (ft_strlen(arg) == 1 && (arg[0] == '+' || arg[0] == '-')))
		{
			ft_putstr_fd("bash: line 1: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			free(arg);
			last_exit_code = 2;
		}
		else
		{
			i = 0;
			sign = 1;
			invalid = 0;
			if (arg[i] == '+' || arg[i] == '-')
			{
				if (arg[i] == '-')
					sign = -1;
				i++;
			}
			num = 0;
			while (arg[i])
			{
				if (!ft_isdigit(arg[i]))
				{
					invalid = 1;
					break;
				}
				prev_num = num;
				num = num * 10 + (arg[i] - '0');
				if (num / 10 != prev_num)  // overflow check
				{
					invalid = 1;
					break;
				}
				i++;
			}

			// Special handling for LLONG_MIN
			if (sign == -1 && strcmp(arg, "-9223372036854775808") == 0)
			{
				num = 9223372036854775808ULL;
				invalid = 0;
			}

			// Check if the number is too large
			if (invalid || (sign == 1 && num > LLONG_MAX) || (sign == -1 && num > 9223372036854775808ULL))
			{
				ft_putstr_fd("bash: line 1: exit: ", STDERR_FILENO);
				ft_putstr_fd(arg, STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
				free(arg);
				last_exit_code = 2;
			}
			else
			{
				long long final_num = (sign == -1) ? -((long long)num) : (long long)num;
				if (cmd->s[2])
				{
					ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
					free(arg);
					return EXIT_FAILURE;
				}
				else
				{
					last_exit_code = (int)(final_num % 256);
					free(arg);
				}
			}
		}
	}
	free_data(data);
	exit(last_exit_code);
}
