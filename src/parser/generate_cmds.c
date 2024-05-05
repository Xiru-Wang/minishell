/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/05/05 20:43:45 by xiruwang         ###   ########.fr       */
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
	data->cmd_num = count_pipe(*token) + 1;//eg. 2 pipe = 3 cmds
	data->cmd_list = NULL;
	while (i < data->cmd_num && *token) // loop thru to generate cmd_list
	{
		new = init_cmd(data);//malloc space
		if (!new)
			free_exit("malloc error", data, EXIT_FAILURE);
		new->id = i;//to identify cmd[last]
		extract_redir(token, new, data);//extract redir and remove from tokenlist
		fill_cmd(token, new);	//fill cmd info && update token list
		append_cmd(&data->cmd_list, new);//append cmd to cmd list
		i++;
	}
	return (data->cmd_list);
}

//loop thru the token list, find redirections and fill the cmd
//once found, remove from token list
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
			del_token(head, temp); // remove redirection sign
			temp = next;
			del_token(head, temp);//remove filename
		}
		//else//if (temp)//!!check if it's NULL before visitS
		temp = temp->next;
	}
}

//assign type to each cmd's redirections
//if multiple '>file1 >file2 > file3'：empty file1,file2, output goes ot files3

static void	add_io_list(t_cmd *cmd, t_token *token)
{
	t_token	*next;
	t_io	*new;

	next = token->next;//if next exsit, call handle_redir
	new = init_io(cmd->data);
	if (token->type == REDIR_IN)
	{
		new->type = REDIR_IN;
		new->filename = ft_strdup(next->value);
	}
	else if (token->type == REDIR_OUT || token->type == APPEND)
	{
		new->type = token->type;
		new->filename = ft_strdup(next->value);
		printf("new->filename:%s\n", new->filename);
	}
	else if (token->type == HEREDOC)
	{
		new->type = HEREDOC;
		if (cmd->delimiter)
				free(cmd->delimiter);
		cmd->delimiter = ft_strdup(next->value);
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
	if (!temp || temp->type == PIPE)//1st cmd cannot be PIPE
		free_exit("syntax error near unexpected token", cmd->data, STDERR_FILENO);
	size = count_args(temp);
	cmd->s = (char **)malloc((size + 1) * sizeof(char *));
	if (!cmd->s)
		free_exit("malloc error", cmd->data, EXIT_FAILURE);
	i = 0;
	while (temp && temp->type != PIPE && size > 0)
	{
		next = temp->next;
		if (temp->type == WORD)
		{
			cmd->s[i] = expand_complex(temp->value, WORD, cmd->data);;
			if (i == 0)
				builtin = ft_bubiltin(cmd->s[0]);
			if (builtin)
				cmd->is_builtin = builtin;
		}
		else if (temp->type == QUO)
			cmd->s[i] = expand_complex(temp->value, QUO, cmd->data);
		del_token(head, temp);
		temp = next;//update temp
		i++;
	}
	cmd->s[i] = NULL;
	if (temp && temp->type == PIPE)//move to next cmd
		del_token(head, temp);
}
