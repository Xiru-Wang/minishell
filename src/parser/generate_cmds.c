/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/04/05 19:23:59 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	extract_redir(t_token **head, t_cmd *cmd, t_data *data);
static void	create_io_list(t_cmd *cmd, t_token *temp);
static void	fill_cmd(t_token **head, t_cmd *cmd);

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
		next = temp->next;
		if (temp->type >= REDIR_IN && temp->type <= HEREDOC)
		{
			if (next == NULL || next->type != WORD || next->type != QUO)
				free_exit("syntax error near unexpected token", data, STDERR_FILENO);
			create_io_list(cmd, temp);
			del_token(head, temp); // remove redirection sign
			temp = next->next;
			del_token(head, next);//remove filename
		}
		if (temp)//!!check if it's NULL before visitS
			temp = temp->next;
	}
}

//assign type to each cmd's redirections
//if multiple '>file1 >file2 > file3'：empty file1,file2, output goes ot files3

static void	create_io_list(t_cmd *cmd, t_token *temp)
{
	t_token	*next;

	next = temp->next;//if next exsit, call handle_redir
	if (!add_io_list(&cmd->io_list))
		free_exit("malloc error", cmd->data, STDERR_FILENO);
	if (temp->type == REDIR_IN)
	{
		cmd->io_list->type = REDIR_IN;
		cmd->io_list->filename = ft_strdup(next->value);
	}
	else if (temp->type == REDIR_OUT || temp->type == APPEND)
	{
		cmd->io_list->type = temp->type;
		cmd->io_list->filename = ft_strdup(next->value);
	}
	else if (temp->type == HEREDOC)
	{
		cmd->io_list->type = HEREDOC;
		if (cmd->delimiter)
				free(cmd->delimiter);
		cmd->delimiter = ft_strdup(next->value);
	}
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
			cmd->s[i] = handle_dollar_quo(temp->value, cmd->data, WORD);;
			if (i == 0)
				builtin = ft_bubiltin(cmd->s[0]);
			if (builtin)
				cmd->is_builtin = builtin;
		}
		else if (temp->type == QUO)
			cmd->s[i] = handle_dollar_quo(temp->value, cmd->data, QUO);
		del_token(head, temp);
		temp = next;//update temp
		i++;
	}
	cmd->s[i] = NULL;
	if (temp && temp->type == PIPE)//move to next cmd
		del_token(head, temp);
}
