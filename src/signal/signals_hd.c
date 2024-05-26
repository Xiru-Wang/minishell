/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_hd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 19:42:08 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/26 13:41:15 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	signal_handler_hd(int signum)
{
	if (signum == SIGINT)
	{
		g_last_signal = 2;
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	readline_event_hook_hd(void)
{
	if (g_last_signal == 2)
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	return (EXIT_SUCCESS);
}

void	init_signals_hd(void)
{
	struct sigaction	sa;
	struct sigaction	sa_quit;

	sa.sa_handler = signal_handler_hd;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_signals_hd(void)
{
	rl_event_hook = readline_event_hook_hd;
	init_signals_hd();
}

void	reset_signals_hd(void)
{
	g_last_signal = 0;
	rl_event_hook = NULL;
	init_signals();
}
