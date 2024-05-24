/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:17 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/24 21:45:25 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_tilde(char *arg)
{
	char	*home;
	char	*expanded;

	if (arg[0] == '~' && (arg[1] == '/' || arg[1] == '\0'))
	{
		home = getenv("HOME");
		expanded = ft_strjoin(home, &arg[1]);
		return (expanded);
	}
	return (NULL);
}

static void	print_arguments(t_cmd *cmd, int start)
{
	char	*expanded_arg;

	while (cmd->s[start])
	{
		if (ft_strncmp(cmd->s[start], "$?", 3) == 0)
			ft_putnbr_fd(cmd->data->exit_code, STDOUT_FILENO);
		else
		{
			expanded_arg = expand_tilde(cmd->s[start]);
			if (expanded_arg)
			{
				ft_putstr_fd(expanded_arg, STDOUT_FILENO);
				free(expanded_arg);
			}
			else
				ft_putstr_fd(cmd->s[start], STDOUT_FILENO);
		}
		if (cmd->s[start + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		start++;
	}
}

static int	is_valid_n_option(char *arg)
{
	int	i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[++i])
		if (arg[i] != 'n')
			return (0);
	return (1);
}

int	call_echo(t_cmd *cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (!cmd->s[1])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
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
