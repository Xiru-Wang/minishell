/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/20 10:02:45 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	init_env(t_data *data, char **env);

void	init_data(t_data *data, char **env)
{
	data->line = NULL;
	data->token_list = NULL;
	data->var_name = NULL;
	data->cmd_list = NULL;
	data->cmd_num = 0;
	data->exit_code = 0;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
        data->pwd = ft_strdup("");
	data->old_pwd = NULL;
	if (init_env(data, env) != EXIT_SUCCESS)
		free_exit("Failed to initialize environment variables.", data, EXIT_FAILURE);
}

static int	init_env(t_data *data, char **env)
{
	int i;

	i = 0;
	while (env[i])
		i++;
	data->env = (char **)malloc(sizeof(char *) * (i + 1));
	if (data->env == NULL)
		return (EXIT_FAILURE);

	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (data->env[i] == NULL)
		{
			while (i > 0)
				free(data->env[--i]);
			free(data->env);
			data->env = NULL;
			return (EXIT_FAILURE);
		}
		i++;
	}
	data->env[i] = NULL;
	return (EXIT_SUCCESS);
}

enum s_type	ft_type(char c)
{
	if (c == '|')
		return (PIPE);
	else if (c == '<')
		return (REDIR_IN);
	else if (c == '>')
		return (REDIR_OUT);
	else if (is_space(c))
		return (_SPACE);
	return (WORD);
}

int	split_line(char *s, t_token **token_list, t_data *data)
{
	int i, res_token;
	enum s_type type;
	int res_quo;

	i = 0;
	while (s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (!s[i])
			break;
		type = ft_type(s[i]);
		if (type == WORD)
		{
			res_quo = check_unclosed_quotes((s + i), token_list);
			if (res_quo == -1)
				free_exit("unclosed quote", data, EXIT_FAILURE);
			i += res_quo;
		}
		else if (type == REDIR_IN && s[i + 1] && ft_type(s[i + 1]) == REDIR_IN)
		{
			res_token = add_token_list(ft_substr(s, i, 2), HEREDOC, token_list);
			if (res_token == 0)
				return (0);
			i += 2;
		}
		else if (type == REDIR_OUT && s[i + 1] && ft_type(s[i + 1]) == REDIR_OUT)
		{
			res_token = add_token_list(ft_substr(s, i, 2), APPEND, token_list);
			if (res_token == 0)
				return (0);
			i += 2;
		}
		else
		{
			res_token = add_token_list(ft_substr(s, i, 1), type, token_list);
			if (res_token == 0)
				return (0);
			i++;
		}
	}
	return (1);
}
