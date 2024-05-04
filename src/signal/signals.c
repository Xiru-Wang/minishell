#include "../../includes/minishell.h"
#include <signal.h>

void sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (isatty(STDIN_FILENO))
		{
			// In interactive mode
			write(STDERR_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
		{
			// In non-interactive mode, terminate the shell process
			write(STDERR_FILENO, "\n", 1);
			exit(EXIT_FAILURE);
		}
	}
	else if (signum == SIGQUIT)
	{
		// In non-interactive mode, ignore SIGQUIT
		if (!isatty(STDIN_FILENO))
			return;
		// In interactive mode, do nothing
	}
}

void	sig_init(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
