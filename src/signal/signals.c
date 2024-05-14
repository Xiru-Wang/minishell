/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jschroed <jschroed@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 19:44:07 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/14 20:26:06 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (isatty(STDIN_FILENO))
		{
			write(STDERR_FILENO, "\n", 2);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
		{
			write(STDERR_FILENO, "\n", 2);
			exit(EXIT_SIGINT);
		}
	}
}

void	sigquit_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		if (isatty(STDIN_FILENO))
		{
		}
		else
		{
			write(STDERR_FILENO, "Quit: 3\n", 8);
			exit(EXIT_SIGQUIT);
		}
	}
}

void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_handler;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
