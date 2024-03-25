/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/03/25 11:55:51 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/*
The strategy for your shell is to have the parent process
do all the piping and redirection before forking the processes.
In this way the children will inherit the redirection.
*/

t_cmd	*generate_cmd(t_token **token, t_data *data)
{
	int		i;
	int		size;
	t_cmd	*new;
	t_token *temp;

	i = 0;
	data->cmd_num = count_pipe(*token) + 1;//eg. 2 pipe = 3 cmds
	data->cmd_list = NULL;
	while (i < data->cmd_num && *token) // loop thru to generate cmd_list
	{
		size = count_args(*token);
		new = creat_cmd(size, data);//malloc space
		if (!new)
			free_exit("malloc error", data, EXIT_FAILURE);
		new->id = i;//to identify cmd[last]
		extract_redir(token, new, data);//extract redir and remove from tokenlist
		fill_cmd(token, new);	//fill cmd info && update token list
		append_cmd(&data->cmd_list, new);//append cmd to cmd list
		i++;
	}
	if (new->id == data->cmd_num)
		data->infile = new->infile;//cmd[last]'s infile is infile for cmd[0]
	return (data->cmd_list);
}

//loop thru the token list, find redirections and fill the cmd
//once found, remove from token list
void extract_redir(t_token **head, t_cmd *cmd, t_data *data)
{
	t_token	*temp;
	t_token	*next;

	temp = *head;
	while (temp && temp->type != PIPE)//&&确保在访问 temp->type 之前 temp 不为 NULL。
	{
		next = temp->next;
		if (temp->type >= REDIR_IN && temp->type <= APPEND)
		{
			if (!next)//nothing behind
				free_exit("parse error \\n\n", data, STDERR_FILENO);
			if (next->type != WORD)//not filename
			{
				printf("parse error near %s\n", next->value);//not sure
				free_exit(NULL, data, STDERR_FILENO);
			}
			handle_redir(cmd, temp);
			del_token(head, temp); // remove redirection sign
			temp = next->next;//temp move to the node after filename if there's
			del_token(head, next);//remove filename
		}
		if (temp)//!!check if it's NULL before visit
			temp = temp->next;
	}
}
/*通过在删除节点后添加 if (temp) 的检查,
可以确保只有在 temp 不为 NULL 时才执行 temp = temp->next 的操作。
这样可以防止在删除节点后访问空指针,提高了代码的健壮性。*/

//assign type to each cmd's redirections
//if multiple '>file1 >file2 >> file3', >>file3 will overwrite outcome??
//check 42's bash!! data stream might be written at all files
//in that case, rewite, have to use linkedlist to save the redirections FUCK
void handle_redir(t_cmd *cmd, t_token *temp)
{
	t_token	*next;

	next = temp->next;//if next exsit, call handle_redir
	if (temp->type == REDIR_IN)
	{
		cmd->in_type = REDIR_IN;
		if (cmd->infile != NULL) // in case multiple <, only take the last one??
			free(cmd->infile);
		cmd->infile = ft_strdup(next->value);
	}
	else if (temp->type == REDIR_OUT || temp->type == APPEND)
	{
		cmd->o_type = temp->type;
		if (cmd->outfile != NULL)
			free(cmd->outfile);
		cmd->outfile = ft_strdup(next->value);
	}
	else if (temp->type == HEREDOC)
	{
		cmd->in_type = HEREDOC;
		if (next->type == WORD)//if delimiter exist
			cmd->delimiter = ft_strdup(next->value);
	}
}

void	fill_cmd(t_token **head, t_cmd *cmd)
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
		if (temp->type == WORD) //( i == 0 && temp->type == WORD)
		{
			cmd->s[i] = ft_strdup(temp->value);//maybe s[0] == -l
			builtin = ft_bubiltin(cmd->s[i]);
			if (builtin)
				cmd->is_builtin = builtin;
		}
		else if (temp->type == ARG)
			cmd->s[i] = ft_strdup(temp->value);
		else if (temp->type == S_QUO)//TBD
			cmd->s[i] = ft_substr(temp->value, 1,ft_strlen(temp->value) - 2);
		else if(temp->type == D_QUO)
			cmd->s[i] = handle_dollar(temp->value, cmd->data);
		del_token(head, temp);
		temp = next;//update temp
		i++;
	}
	cmd->s[i] = NULL;//terminate 2d array. now i == size
	if (temp && temp->type == PIPE)//move to next cmd
		del_token(head, temp);
}
