# minishell

```c
#include "../../includes/minishell.h"

int last_received_signal = 0;

void signal_handler(int signum) {
    last_received_signal = signum;
}

void handle_interrupt(int *special_handling) {
    if (special_handling && *special_handling) {
        // Custom handling of SIGINT in the specific function
        write(STDERR_FILENO, "Special SIGINT handling\n", 24);
        // Perform any additional actions or cleanup
    } else {
        // Default handling of SIGINT
        write(STDERR_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void handle_quit() {
    if (isatty(STDIN_FILENO)) {
        write(STDERR_FILENO, "Quit signal received, but ignored.\n", 35);
    }
}

int signal_check(int *special_handling) {
    if (last_received_signal != 0) {
        if (last_received_signal == SIGINT)
            handle_interrupt(special_handling);
        else if (last_received_signal == SIGQUIT)
            handle_quit();
        last_received_signal = 0;
    }
    return 0;
}

void init_signal(int *special_handling) {
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    rl_event_hook = (int (*)(void))signal_check(special_handling);
}

void specific_function(t_data *data) {
    int special_handling = 1;
    init_signal(&special_handling);
    // Code for the specific function where you want to handle SIGINT differently
    // ...
    special_handling = 0;
}

void minishell(t_data *data)
{
    char    *s;

    init_signal(NULL);

    while (1)
    {
        s = readline("minishell>>");
        if (!s)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        // Rest of the code...

        if (/* condition to call specific_function */) {
            specific_function(data);
        }
    }
    free_data(data);
    exit(EXIT_SUCCESS);
}
```


#readline
````
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>

//cc -lreadline readline.c

/*
readline will read a line from the terminal and return it, using prompt as a prompt.
If prompt is NULL or the empty string, no prompt is issued.
The line returned is allocated with malloc(3);
the caller must FREE it when finished.
*/

int main(int ac, char **av)
{
	while(1)
	{
		char *input;
		input = readline("minishell>>");
		add_history(input);
		printf("%s\n", input);
		free(input);
	}
	return 0;
}
````

