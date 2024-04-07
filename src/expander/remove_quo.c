#include "../../includes/minishell.h"

static int	count_valid_quotes(char *s)
{
	int		i;
	int		count;
	char	c;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				i++;
			if (s[i] == c)
				i++;
			count += 2;
		}
		i++;
	}
	return (count);
}

char	*remove_quo(char *s)
{
	int		i;
	int		k;
	char	*new;
	char	c;
	int		count;

	i = 0;
	k = 0;
	count = count_valid_quotes(s);
	new = (char *)malloc(ft_strlen(s) - count + 1);
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				new[k++] = s[i++];
			if (s[i] == c)
				i++;
		}
		new[k++] = s[i++];
	}
	new[k] = '\0';
	return (new);
}
