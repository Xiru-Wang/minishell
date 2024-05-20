/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/20 14:00:08 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	extract_redir(t_token **head, t_cmd *cmd);
static void	add_io_list(t_cmd *cmd, t_token *temp);
static int	fill_cmd(t_token **head, t_cmd *cmd);


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
		if (extract_redir(token, new) == 1)
		{
			data->exit_code = 2;
			return (data->cmd_list);
		}
		if (fill_cmd(token, new) == 1)
		{
			data->exit_code = 2;
			return (data->cmd_list);
		}
		append_cmd(&data->cmd_list, new);
		i++;
	}
	return (data->cmd_list);
}

//find redirection token and the next token(filename)
//add to io_list, remove from token_list
static int	extract_redir(t_token **head, t_cmd *cmd)
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
			{
				if (!next || !next->value)
					printf("minishell: syntax error near unexpected token `newline\'\n");
				else	
					printf("minishell: syntax error near unexpected token `%s\'\n", next->value);
				return (EXIT_FAILURE);
			}
			add_io_list(cmd, temp);
			del_token(head, temp);
			del_token(head, next);
			temp = *head;
		}
		else
			temp = temp->next;
	}
	return (EXIT_SUCCESS);
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
		new->filename = remove_quo(next->value);
	}
	else if (token->type == REDIR_OUT || token->type == APPEND)
	{
		new->type = token->type;
		new->filename = remove_quo(next->value);
	}
	else if (token->type == HEREDOC)
	{
		new->type = HEREDOC;
		new->eof = ft_strdup(next->value);
	}
	append_io(&cmd->io_list, new);
}

static int	fill_cmd(t_token **head, t_cmd *cmd)
{
	t_token		*temp;
	t_token		*next;
	int			i;
	t_builtin	builtin;
	int			size;

	temp = *head;
	if (!temp || temp->type == PIPE)
	{
		if (!temp || !temp->value)
			printf("minishell: syntax error near unexpected token `newline\'\n");
		else	
			printf("minishell: syntax error near unexpected token `%s\'\n", temp->value);
		return (EXIT_FAILURE);
	}
	size = count_args(temp) + 1;
	cmd->s = (char **)ft_calloc(size, sizeof(char *));
	if (!cmd->s)
		free_exit("malloc error", cmd->data, EXIT_FAILURE);
	i = 0;
	while (temp && temp->type != PIPE && size > 0)
	{
		next = temp->next;
		cmd->s[i] = NULL;
		if (temp->type == WORD)
			cmd->s[i] = expand_complex(temp->value, WORD, cmd->data);
		else if (temp->type == QUO)
			cmd->s[i] = expand_complex(temp->value, QUO, cmd->data);
		if (i == 0)
		{
			builtin = ft_builtin(cmd->s[0]);
			if (builtin)
				cmd->is_builtin = builtin;
		}
		del_token(head, temp);
		temp = next;
		i++;
	}
	cmd->s[i] = NULL;
	if (temp && temp->type == PIPE)
		del_token(head, temp);
	return (EXIT_SUCCESS);
}
