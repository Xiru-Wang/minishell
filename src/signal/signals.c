#include "../../includes/minishell.h"

// HEREDOC

int g_last_signal = 0;

void signal_handler_hd(int signum) {
	if (signum == SIGINT) {
		g_last_signal = 1;
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int readline_event_hook_hd() {
	if (g_last_signal == 1) {
		rl_done = 1;
	}
	return 0;
}

void	init_signals_hd(void)
{

	struct sigaction sa;
	sa.sa_handler = signal_handler_hd;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

// MINISHELL

void signal_handler(int signum) {
	if (signum == SIGINT) {
		write(STDERR_FILENO, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void init_signals(void) {
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void setup_signals_hd() {
	rl_event_hook = readline_event_hook_hd;
	init_signals_hd();
}

void reset_signals_hd() {
	g_last_signal = 0;
	rl_event_hook = NULL;
	init_signals();
}
