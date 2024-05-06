#include "../../includes/minishell.h"

void sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (g_in_heredoc)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if (isatty(STDIN_FILENO))
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
	else if (signum == SIGQUIT)
	{
		if (!isatty(STDIN_FILENO))
			return;
	}
}

void	init_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
