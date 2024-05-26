/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:47:18 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 19:38:26 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	init_env(t_data *data, char **env);
static int	handle_word_token(char *s, int i, t_token **head, t_data *data);

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
		free_exit("Failed to initialize environment variables.", \
		data, EXIT_FAILURE);
}

static int	init_env(t_data *data, char **env)
{
	int	i;

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

enum s_type	tk_type(char c)
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

void	split_line(char *s, t_token **head, t_data *data)
{
	int			i;
	enum s_type	tp;

	i = 0;
	while (s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (!s[i])
			return ;
		tp = tk_type(s[i]);
		if (tp == WORD)
			i = handle_word_token(s, i, head, data);
		else if (tp == REDIR_IN && s[i + 1] && tk_type(s[i + 1]) == REDIR_IN)
			i += add_token_list(ft_substr(s, i, 2), HEREDOC, head, data);
		else if (tp == REDIR_OUT && s[i + 1] && tk_type(s[i + 1]) == REDIR_OUT)
			i += add_token_list(ft_substr(s, i, 2), APPEND, head, data);
		else
			i += add_token_list(ft_substr(s, i, 1), tp, head, data);
	}
}

static int	handle_word_token(char *s, int i, t_token **head, t_data *data)
{
	int	res;

	res = check_unclosed_quotes((s + i), head, data);
	if (res == -1)
		free_exit("unclosed quote", data, EXIT_FAILURE);
	return (i + res);
}
