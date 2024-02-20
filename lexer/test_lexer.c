#include "../../includes/minishell.h"

int	is_special(char c)
{
	if (c == "|" || c == ">" || c == "<")
		return (1);
	return (0);
}

//think about << >> and quotes
int	count_num_token(char *s)
{
	int n;

	n = 0;
	while (*s)
	{
		if (!ft_is_space(*s) && !is_special(*s))//not | < > << >>
		{
			n++;
			while (!ft_is_space(*s) && !is_special(*s))
				s++;
		}
		while (*s && ft_is_space(*s))
			s++;
		if (is_special(*s))
			n++;
	}
	return (n + 1);
}

char	**split_line(const char *line, t_session *session)
{
	char *new;
	char **tokens;
	int start;
	int end;

	//include \n??🤔
	new = ft_strtrim(line, " \t\n\v\f\r"); // remove empty char at start and end
	session->num_tokens = count_num_tokens(new);
	tokens = (char **)malloc(sizeof(char **) * session->num_tokens);//??do i need?
	if (!tokens)
		return (NULL);
	int i = 0;
	int n = 0;
	while (new[i])
	{
		if (!ft_is_space(new[i]) && !is_special(new[i]))//not | < > << >>
		{
			start = i;
			while (!ft_is_space(new[i]) && !is_special(new[i]))//skip rest of the word
				i++;
		}
		if (ft_is_space(new[i]) || is_special(new[i]))//only count 1 space
		{
			end = i;
			tokens[n] = ft_substr(new, start, end - start);
			n++;
		}
		i++;
		while (ft_is_space(new[i]) && new[i])//skip extra space??
			i++;
	}
	tokens[session->num_tokens] = NULL;//terminate 1st dimension
	return (tokens);
}
