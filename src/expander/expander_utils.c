
#include "../../includes/minishell.h"

int	check_valid_dollar(char *s)//check whole string
{
	while (*s)
	{
		if (*s == '$' && *(s + 1))
		{
			s++;
			if (char_is_valid(*s))
				return (1);
		}
		s++;
	}
	return (0);
}

int	char_is_valid(char c)//check the char after $
{
	if (ft_isalnum(c) || c == '_' || c == '?')
		return (1);
	else
		return (0);
}

// if  "'$USER'" -> '$USER'
int	check_valid_dollar_limit(char *s, int max)//check max len of the string
{
	int	i;

	i = 0;
	while (s[i] && i < max)
	{
		if (s[i] == '$' && s[i + 1])
		{
			i++;
			if (char_is_valid(*s))
				return (1);
		}
		i++;
	}
	return (0);
}
