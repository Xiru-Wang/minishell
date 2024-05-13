#include "../../includes/minishell.h"

int g_last_signal = 0;
ShellContext current_context;

void signal_handler(int signum) 
{
	if (signum == SIGINT) {
		g_last_signal = 1;
		if (current_context == CONTEXT_HEREDOC) 
		{
			rl_replace_line("", 0);
			rl_redisplay();
		} 
		else 
		{
			write(STDERR_FILENO, "\n", 2);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

int readline_event_hook() 
{
	if (g_last_signal == 1)
		rl_done = 1;
	return 0;
}

void init_signals(ShellContext context) 
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	current_context = context;
	if (context == CONTEXT_HEREDOC)
		rl_event_hook = readline_event_hook;
	else
		rl_event_hook = NULL;
}

void reset_signals() {
	init_signals(CONTEXT_MAINSHELL);
}
