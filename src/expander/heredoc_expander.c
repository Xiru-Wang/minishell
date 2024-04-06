#include "../../includes/minishell.h"
//for here_doc
//var expand even if in the single quote
//static char	*remove_char(char *s, int index);
//static char	*replacer(char *s, int *i, t_data *data);

//in this function: vars expand without careing quotes
char *replace_vars_simple(char *s, t_data *data)
{
	int     i, var_len, flag, k;
    char    *dst, *value, *temp;

    flag = 0;
    dst = ft_calloc(1, 1);
    i = 0;
    while (s[i])
    {
		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
        {
            flag = 1;
            k = 0;
            value = expand_dollar(s + i, &k, data);
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

/*
char	*replace_vars_simple(char *s, t_data *data)
{
	char	*temp;
	char	*new;
	int		i;

	i = 0;
	temp = s;
	while (temp[i])
	{
		if (temp[i] == '$' && temp [i + 1] && char_is_valid(temp[i + 1]))
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
/*
static char	*replacer(char *s, int *i, t_data *data)
{
	char	*s1, *s2;
	char	*value, *var_name, *new, *temp;
	int		start, end;
	int		len;

	start = *i + 1; // skip $
	len = 0;
	while (ft_isalnum(s[start + len]) || s[start + len] == '_')
		len++; // USER:4
	end = start + len; // 11
	var_name = ft_substr(s, start, len); // USER
	value = find_var(var_name, len, data->env); // replace USER
	free(var_name);
	s1 = ft_substr(s, 0, *i); // 6chars:"hello*"
	s2 = ft_substr(s, end, ft_strlen(s) - end); // 14-11=3char:"*42"
	if (!value) // not exist, remove $var_name
	{
		new = ft_strjoin(s1, s2);
		*i = end;
	}
	else
	{
		temp = ft_strjoin(s1, value);
		new = ft_strjoin(temp, s2);
		free(temp);
		*i = end + ft_strlen(value);
		free(value);
	}
	free(s1);
	free(s2);
	return (new);
}
*/
