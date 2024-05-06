#include "../../includes/minishell.h"

int last_received_signal = 0;

void signal_handler(int signum) {
	last_received_signal = signum;
}

void handle_interrupt() 
{
	if (isatty(STDIN_FILENO)) 
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else 
	{
		write(STDERR_FILENO, "\n", 1);
		exit(EXIT_FAILURE);
	}
}

void handle_quit() 
{
	if (!isatty(STDIN_FILENO))
		return;
}

int readline_event_hook_signals() 
{
	if (last_received_signal != 0) {
		if (last_received_signal == SIGINT)
			handle_interrupt();
		else if (last_received_signal == SIGQUIT)
			handle_quit();
		last_received_signal = 0;
	}
	return (0);
}

/* void signal_handler(int signum) */
/* { */
/*     if (signum == SIGINT) */
/*     { */
/*         if (isatty(STDIN_FILENO)) */
/*         { */
/*             write(STDERR_FILENO, "\n", 1); */
/*             rl_on_new_line(); */
/*             rl_replace_line("", 0); */
/*             rl_redisplay(); */
/*         } */
/*         else */
/*         { */
/*             write(STDERR_FILENO, "\n", 1); */
/*             exit(EXIT_FAILURE); */
/*         } */
/*     } */
/*     else if (signum == SIGQUIT) */
/*     { */
/*         if (!isatty(STDIN_FILENO)) */
/*             return; */
/*     } */
/* } */

void	init_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	rl_event_hook = readline_event_hook_signals;
}
