#include "../../includes/minishell.h"

/* int last_received_signal = 0; */
//volatile sig_atomic_t last_received_signal = 0;

int g_last_signal = 0;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        g_last_signal = 1;
		rl_replace_line("", 0);  // Clear the current input line
		rl_redisplay();  // Redisplay the prompt
    }
}

int readline_event_hook() {
    if (g_last_signal == 1) {
		rl_done = 1;  // Set rl_done to indicate readline should return
    }
    return 0;
}

// void	init_heredoc_sig(int sig)
// {
// 	(void)sig;
// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	//g_exit = 130;
// }

// void handle_interrupt()
// {
// 	if (isatty(STDIN_FILENO))
// 	{
// 		write(STDERR_FILENO, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// 	else
// 	{
// 		write(STDERR_FILENO, "\n", 1);
// 		exit(EXIT_FAILURE);
// 	}
// }

// void	handle_quit()
// {
// 	if (!isatty(STDIN_FILENO))
// 		return;
// }

// int readline_event_hook_signals()
// {
// 	if (g_last_signal != 0)
// 	{
// 		if (g_last_signal == SIGINT)
// 			handle_interrupt();
// 		else if (g_last_signal == SIGQUIT)
// 			handle_quit();
// 		g_last_signal = 0;
// 	}
// 	return (0);
// }

// void	init_signal(void)
// {
// 	struct sigaction	sa;

// 	ft_memset(&sa, 0, sizeof(struct sigaction));
// 	sa.sa_handler = signal_handler;
// 	sa.sa_flags = SA_RESTART;
// 	sigemptyset(&sa.sa_mask);
// 	sigaction(SIGINT, &sa, NULL);
// 	sigaction(SIGQUIT, &sa, NULL);
// 	rl_event_hook = readline_event_hook_signals;
// }
