/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:37:58 by xiruwang          #+#    #+#             */
/*   Updated: 2024/02/17 13:28:23 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	lexer(t_session *data, char **env)
{
	char	**tokens;

	if (data->input == '\0')//?check before this step?
		return (0);
	if (if_all_space(data->input) == 1)//if whole string is space
		return (0);
	tokens = split_input(data->input);
	if (!tokens)
		return (0);
	//todo Command Substitution eg. echo $(hi)
	//cmd_substitution(tokens, env);
	data->token = tokenizer(tokens);
	free_double_ptr(tokens);
	if (!data->token)
		return (0);
	return (1);
}

char	**split_input(char *line)
{
	char **new;
	char *arg;
	char *s;
	int i = 0;

	s = ft_strdup(line);//make sure do not alter original string from readline()
	if (!s)
		return (NULL);
	//todo
	//remove $ or quotes
	new = NULL;
	while (*s)
	{
		//skip spaces at the begining
		while (ft_is_space(*s))//ft_strtrim
			s++;
		// while *s && *s != space && *s != OPERATOR
		// strlcpy?
		// if *s == space || *s == OPERATOR
		//split + terminate
	}
	free(s);
	return (new);
}
