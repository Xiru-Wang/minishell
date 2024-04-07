#include "../../includes/minishell.h"
//for here_doc
//var expand even if in the single quote
//static char	*remove_char(char *s, int index);
//static char	*replacer(char *s, int *i, t_data *data);

//in this function: vars expand without careing quotes
char *replace_vars_simple(char *s)
{
	int     i;
	int		k;
	int		flag;
	char    *dst;
	char	*value;

	flag = 0;
	dst = ft_calloc(1, 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
		{
			flag = 1;
			k = 0;
			value = expand_dollar(s + i, &k);
			if (value)
				combine_strings(dst, s, value, &i, k);
			else
				i = i + k;
		}
		i++;
	}
	if (flag)
		return (dst);
	free(dst);
	return (ft_strdup(s));
}
