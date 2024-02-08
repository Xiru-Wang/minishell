# minishell

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
