/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/27 17:31:51 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	extract_redir(t_token **head, t_cmd *cmd);
static void	add_io_list(t_cmd *cmd, t_token *temp);
static int	fill_cmd(t_token **head, t_cmd *cmd);
static int	fill_args(t_token **head, int size, t_cmd *cmd);

// input:			ls -l | grep 'hi' > test.txt
// token_list:		"ls", "-l", "|", "grep", "'hi'", ">", "test.txt"
// cmd0:			"ls -l",
// cmd1: io_list:	"> test.txt", "grep hi"
int	generate_cmds(t_token **token, t_cmd **cmd_list, t_data *data)
{
	int		i;
	t_cmd	*new;

	i = 0;
	if (check_empty_token(*token, data) || pipe_syntax(*token, data))
		return (EXIT_FAILURE);
	data->cmd_num = count_pipe(*token) + 1;
	while (i < data->cmd_num && *token)
	{
		new = init_cmd(data);
		if (!new)
			free_exit("malloc error", data, EXIT_FAILURE);
		if (extract_redir(token, new) == EXIT_FAILURE || \
			fill_cmd(token, new) == EXIT_FAILURE)
		{
			data->exit_code = 2;
			free(new);
			return (EXIT_FAILURE);
		}
		append_cmd(cmd_list, new);
		i++;
	}
	return (EXIT_SUCCESS);
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
			if (check_syntax(next, cmd->data) == EXIT_FAILURE)
				return (EXIT_FAILURE);
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
		new->filename = expand_complex(next->value, cmd->data);
	}
	else if (token->type == REDIR_OUT || token->type == APPEND)
	{
		new->type = token->type;
		new->filename = expand_complex(next->value, cmd->data);
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
	int			size;

	temp = *head;
	if (!temp)
		return (0);
	if (temp->type == PIPE)
		return (EXIT_SUCCESS);
	size = count_args(temp) + 1;
	cmd->s = (char **)ft_calloc(size, sizeof(char *));
	if (!cmd->s)
		free_exit("malloc error", cmd->data, EXIT_FAILURE);
	fill_args(head, size, cmd);
	return (EXIT_SUCCESS);
}

static int	fill_args(t_token **head, int size, t_cmd *cmd)
{
	t_token		*next;
	t_token		*temp;
	int			i;

	temp = *head;
	i = 0;
	while (temp && temp->type != PIPE && size > 0)
	{
		next = temp->next;
		if (temp->type == STR)
		{
			expand_arg(temp, next, cmd, &i);
			del_token(head, temp);
		}
		temp = next;
		size--;
	}
	cmd->s[i] = NULL;
	if (temp && temp->type == PIPE)
		del_token(head, temp);
	return (0);
}
