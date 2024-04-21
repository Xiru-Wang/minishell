#include "../../includes/minishell.h"

t_global g_exit_status;

static int		event(void) {
	return EXIT_SUCCESS;
}

static void	sigint_handler(int sig) 
{
	if (!g_exit_status.in_heredoc)
		ft_putstr_fd("\n", STDERR_FILENO);
	if (g_exit_status.in_cmd) 
	{
		g_exit_status.stop_heredoc = 1;
		rl_replace_line("", 0);
		rl_redisplay();
		rl_done = 1;
	}
	else 
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	(void)sig;
}

static void	sigquit_handler(int sig) 
{
	ft_putstr_fd("\nQuit: ", STDERR_FILENO);
	ft_putnbr_fd(sig, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(EXIT_SUCCESS);
}

void	init_signals(void) 
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	rl_event_hook = event;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

