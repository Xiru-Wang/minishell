#include "../../includes/minishell.h"

// TODO: FIX

/*

### expand_simple ###

[----] tests/test_expand_simple.c:75: Assertion Failed
[----]
[----]   Expected: Hello, '$USER', "John"!, Got: Hello, 'John', "John"!
[----]
[----] tests/test_expand_simple.c:55: Assertion Failed
[----]
[----]   Expected: Hello, '$USER'!, Got: Hello, 'John'!
[----]
[FAIL] expand_simple::mixed_quotes: (0.00s)
[FAIL] expand_simple::single_quotes: (0.00s)
[====] Synthesis: Tested: 6 | Passing: 4 | Failing: 2 | Crashing: 0

minishell_maomi on  main [!?]
❯ echo Hello, '$USER', "$USER"!
Hello, $USER, janschroeder!

*/

static int	count_var_len(char *var);

// in this function: vars expand without careing quotes
char	*expand_simple(char *s, char **env)
{
	int		i;
	int		k;
	char	*dst;
	char	*value;

	dst = ft_calloc(sizeof(char), 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] && char_is_valid(s[i + 1]))
		{
			k = 0;
			value = expand_dollar(s + i, &k, env);
			ft_strlcat(dst, value, ft_strlen(dst) + ft_strlen(value) + 1);
			free(value);
			i = i + k;
		}
		value = char_to_str(s[i]);
		ft_strlcat(dst, value, ft_strlen(dst) + 2);
		free(value);
		i++;
	}
	return (dst);
}

int	len_within_quo(char *s, char c)
{
	int	len;

	len = 0;
	if (*s == c)
	{
		len++;
		while (*s && *s != c)
			len++;
		if (*s == c)
			len++;
	}
	return (len);
}

char	*expand_dollar(char *s, int *len, char **env)
{
	int		var_len;
	char	*var_name;
	char	*value;

	value = NULL;
	if (*s == '$')
	{
		s++; // skip dollar
		if (ft_isdigit(*s)) // skip 1st digit
		{
			*len = 2;
			return (NULL);
		}
		/* if (*s == '?') // skip 1st digit */
		/* { */
		/*     *len = 2; */
		/*     value = ft_itoa(g_exit_code); */
		/*     return (value); */
		/* } */
		var_len = count_var_len(s);
		var_name = ft_substr(s, 0, var_len);
		value = find_env(var_name, env);
		free(var_name);
		*len = 1 + var_len; // include the $ in the length
	}
	return (value);
}

static int	count_var_len(char *var)
{
    int i;

    i = 0;
    while (ft_isalnum(var[i]) || var[i] == '_')
        i++;
    return (i);
}
