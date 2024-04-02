/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:17 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/02 19:51:43 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//echo
//with -n: no new line
//without -n: new line
//s[1] has to be -n (with a random number of n's) eg. -nnnn->works
//eg> echo -nn -nnnn2 -n "hi " "dad" "blala    lala"
//-nnnn2 -n hi  dad blala    lala%

void	call_echo(t_cmd *cmd)
{
	int	i;
	int	k;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd->s[i++])//skip valid -n args
	{
		k = 1;
		if (cmd->s[i][0] == '-' && cmd->s[i][1] == 'n')//first arg must be -n
		{
			while(cmd->s[i][k] == 'n')//-nnnnn skip rest n
				k++;
			if (cmd->s[i][k] == '\0')//-n2 wrong
				flag = 1;
			else
				break ;
		}
	}
	while (cmd->s[i++])
	{
		// if (!ft_strncmp(cmd->s[i], "$?", 2))//??if single quote?
		// 	ft_putnbr_fd(g_exit_code, STDOUT_FILENO);
		ft_putstr_fd(cmd->s[i], STDOUT_FILENO);
		if (cmd->s[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (flag == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
