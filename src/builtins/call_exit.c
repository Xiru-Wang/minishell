/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 12:37:05 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/26 10:14:28 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_arg(char *arg);
static int	calculate_exit_code(char *arg);
static char	*trim_and_validate_arg(char *arg, int *last_exit_code);
static int	check_and_convert_number(char *arg, unsigned long long *num);

int	call_exit(t_cmd *cmd, t_data *data)
{
	int		last_exit_code;
	char	*arg;
	int		i;

	last_exit_code = data->exit_code;
	arg = NULL;
	i = 1;
	if (!cmd->s[i])
		free_exit("exit", data, EXIT_SUCCESS);
	if (cmd->s[i] && empty_str_exit(cmd->s[i]) == 1)
		free_exit(NULL, data, 2);
	if (cmd->s[i] && ft_strncmp(cmd->s[i], "--", 3) == 0)
		i++;
	if (cmd->s[i])
	{
		arg = trim_and_validate_arg(cmd->s[i], &last_exit_code);
		if (arg && cmd->s[i + 1])
			return (too_many_arguments_exit(arg));
		else if (arg)
			last_exit_code = calculate_exit_code(arg);
		free(arg);
	}
	free_data(data);
	exit(last_exit_code);
}

static char	*trim_and_validate_arg(char *arg, int *last_exit_code)
{
	char	*trimmed_arg;

	trimmed_arg = ft_strtrim(arg, " \t");
	if (!is_valid_arg(trimmed_arg))
	{
		*last_exit_code = print_error_exit(trimmed_arg);
		free(trimmed_arg);
		return (NULL);
	}
	return (trimmed_arg);
}

static int	is_valid_arg(char *arg)
{
	int	i;

	i = 0;
	if (ft_strlen(arg) == 0 || (ft_strlen(arg) == 1 && (arg[0] == '+' || \
					arg[0] == '-')))
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	calculate_exit_code(char *arg)
{
	int					sign;
	int					i;
	unsigned long long	num;

	sign = 1;
	i = 0;
	num = 0;
	if (arg[i] == '+' || arg[i] == '-')
		if (arg[i++] == '-')
			sign = -1;
	if (!check_and_convert_number(arg + i, &num))
		return (print_error_exit(arg));
	if (sign == -1 && ft_strncmp(\
				arg, "-9223372036854775808", ft_strlen(arg)) == 0)
		num = 9223372036854775808ULL;
	else if ((sign == 1 && num > LLONG_MAX) || \
			(sign == -1 && num > 9223372036854775808ULL))
		return (print_error_exit(arg));
	if (sign == -1)
		return ((int)(-((long long)num) % 256));
	return ((int)((long long)num % 256));
}

static int	check_and_convert_number(char *arg, unsigned long long *num)
{
	int					i;
	unsigned long long	prev_num;

	i = 0;
	*num = 0;
	while (arg[i])
	{
		prev_num = *num;
		*num = *num * 10 + (arg[i] - '0');
		if (*num / 10 != prev_num)
		{
			return (0);
		}
		i++;
	}
	return (1);
}
