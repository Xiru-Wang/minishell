/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_noninteractive.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 19:44:07 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/26 15:58:27 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	sigint_handler_noint(int signum)
{
	if (signum == SIGINT)
	{
		if (isatty(STDIN_FILENO))
		{
			write(STDERR_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
		{
			write(STDERR_FILENO, "\n", 1);
			exit(EXIT_SIGINT);
		}
	}
	if (signum == SIGQUIT)
	{
		if (isatty(STDIN_FILENO))
		{
			write(1, "Quit (core dumped)", ft_strlen("Quit (core dumped)"));
			write(1, "\n", 1);
			rl_on_new_line();
		}
	}
}

void	init_signals_noint(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler_noint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigint_handler_noint;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
