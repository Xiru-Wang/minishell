#include "../../includes/minishell.h"
//for here_doc
//var expand even if in the single quote
char	*replace_vars_simple(char *s, t_data *data)
{
	char	*temp;
	char	*new;
	int		i;

	i = 0;
	temp = s;
	while (temp[i])
	{
		if (temp[i] == '$' && check_valid_dollar(temp + i))
		{
			i++;
			if (ft_isdigit(temp[i]))
				new = remove_char(s, i);
			else if (ft_isalpha(temp[i]) || temp[i] == '_')
				new = replacer(s, &i, data);//if var found i increase len of env
			if (new && new != temp)//if more than one $
			{
				if (temp != s)
					free(temp);
			}
			temp = new;
		}
		i++;
	}
	return (temp);
}

static char	*remove_char(char *s, int index)
{
	int		i, k;
	char	*new;

	new = (char *)malloc(ft_strlen(s) - 2 + 1);
	i = 0;
	k = 0;
	while (s[i])
	{
		if (i == index)
			i = i + 2;//skip $ sign and 1 digit
		new[k] = s[i];
		k++;
		i++;
	}
	new[k] = 0;
	return (new);
}

/*
eg. "hello*$USER*42" * . () ！etc,will be sepreator
     |  s1 |var|s2|
start = 6: $
len(var) = 5
end = 11
strlen(s)=14
*/
static char	*replacer(char *s, int *i, t_data *data)
{
	char	*s1, *s2;
	char	*value, *var_name, *new, *temp;
	int		start, end;
	int		len;

	start = *i + 1;//skip $
	len = 0;
	while (ft_isalnum(s[start + len]) || s[start + len] == '_')
		len++;//USER:4
	end = start + len;// 11
	var_name = ft_substr(s, start, len);//USER
	value = find_var(var_name, data->env);//replace USER
	free(var_name);
	s1 = ft_substr(s, 0, *i);// 6chars:"hello*"
	s2 = ft_substr(s, end, ft_strlen(s) - end);//14-11=3char:"*42"
	if (!value) // not exsit, remove $var_name
		new = ft_strjoin(s1, s2);
	else
	{
		temp = ft_strjoin(s1, value);
		new = ft_strjoin(temp, s2);
		free(temp);
		*i += ft_strlen(value);
		free(value);
	}
	free(s1);
	free(s2);
	return (new);
}
