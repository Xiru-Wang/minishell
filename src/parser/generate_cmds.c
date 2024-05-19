/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/19 18:19:29 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	extract_redir(t_token **head, t_cmd *cmd, t_data *data);
static void	add_io_list(t_cmd *cmd, t_token *temp);
static void	fill_cmd(t_token **head, t_cmd *cmd);


// input:			ls -l | grep 'hi' > test.txt
// token_list:		"ls", "-l", "|", "grep", "'hi'", ">", "test.txt"
// cmd0:			"ls -l",
// cmd1: io_list:	"> test.txt", "grep hi"

t_cmd	*generate_cmds(t_token **token, t_data *data)
{
	int		i;
	t_cmd	*new;

	i = 0;
	data->cmd_num = count_pipe(*token) + 1;
	data->cmd_list = NULL;
	while (i < data->cmd_num && *token)
	{
		new = init_cmd(data);
		if (!new)
			free_exit("malloc error", data, EXIT_FAILURE);
		extract_redir(token, new, data);
		fill_cmd(token, new);
		append_cmd(&data->cmd_list, new);
		i++;
	}
	return (data->cmd_list);
}

//find redirection token and the next token(filename)
//add to io_list, remove from token_list
static void	extract_redir(t_token **head, t_cmd *cmd, t_data *data)
{
	t_token	*temp;
	t_token	*next;

	temp = *head;
	while (temp && temp->type != PIPE)
	{
		if (temp->type >= REDIR_IN && temp->type <= HEREDOC)
		{
			next = temp->next;
			if (temp->next == NULL || (next->type != WORD && next->type != QUO))
				free_exit("syntax error near unexpected token", data, STDERR_FILENO);
			add_io_list(cmd, temp);
			del_token(head, temp);
			del_token(head, next);
			temp = *head;
		}
		else
			temp = temp->next;
	}
}

//assign type to each cmd's redirections
//if multiple '>file1 >file2 > file3'：empty file1,file2, output goes to files3
static void	add_io_list(t_cmd *cmd, t_token *token)
{
	t_token	*next;
	t_io	*new;

	next = token->next;
	new = init_io(cmd);
	if (token->type == REDIR_IN)
	{
		new->type = REDIR_IN;
		new->filename = ft_strdup(next->value);
	}
	else if (token->type == REDIR_OUT || token->type == APPEND)
	{
		new->type = token->type;
		new->filename = ft_strdup(next->value);
	}
	else if (token->type == HEREDOC)
	{
		new->type = HEREDOC;
		new->eof = ft_strdup(next->value);
	}
	append_io(&cmd->io_list, new);
}

static void	fill_cmd(t_token **head, t_cmd *cmd)
{
	t_token		*temp;
	t_token		*next;
	int			i;
	t_builtin	builtin;
	int			size;

	temp = *head;
	if (!temp || temp->type == PIPE)
		free_exit("syntax error near unexpected token", cmd->data, STDERR_FILENO);
	size = count_args(temp) + 1;
	cmd->s = (char **)ft_calloc(size, sizeof(char *));
	if (!cmd->s)
		free_exit("malloc error", cmd->data, EXIT_FAILURE);
	i = 0;
	while (temp && temp->type != PIPE && size > 0)
	{
		next = temp->next;
		// if (cmd->s[i])//Had freed in minishell loop
		// 	free(cmd->s[i]);
		cmd->s[i] = NULL;
		if (temp->type == WORD)
		{
			cmd->s[i] = expand_complex(temp->value, WORD, cmd->data);;
			if (i == 0)
			{
				builtin = ft_bubiltin(cmd->s[0]);
				if (builtin)
					cmd->is_builtin = builtin;
			}
		}
		else if (temp->type == QUO)
		{
			cmd->s[i] = NULL;
			cmd->s[i] = expand_complex(temp->value, QUO, cmd->data);
		}
		del_token(head, temp);
		temp = next;
		i++;
	}
	cmd->s[i] = NULL;
	if (temp && temp->type == PIPE)
		del_token(head, temp);
}
