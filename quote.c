#include "includes/minishell.h"

// previos call: handle_quotes((s+i), QUOTE/D_QUO, head, n)
int	handle_quotes(char *s, int quote, t_token **head, int n)
{
	int	i;

	i = 0;
	//IN THE LOOP, only if s[0] == quote
	while (s[i])
	{
		i++;
		while (s[i] && s[i] != quote)//count the length till next quote
			i++;
		if (s[i] != quote)//unclosed
			return (perror("quotes unclosed"), STDERR);
		add_list(ft_substr(s, 1, i - 1), quote, head, n);
	}
	return (i + 2);//+2 in order to skip a pair of quotes
}
