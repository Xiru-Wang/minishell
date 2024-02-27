#include "includes/minishell.h"

// IN THE LOOP, only if s[0] == quote
int	handle_quotes(char *s, int quote, t_token **head, int n)
{
	int	i;

	i = 1;
	while (s[i] && s[i] != s[0])//Look for the closing quote
		i++;
	if (s[i] == s[0])
	{
		add_list(ft_substr(s, 1, i - 1), quote, head, n);
		return (i + 1);// Return the position after the closing quote
	}
	else
		return (-1);
}
