/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:50:42 by xiruwang          #+#    #+#             */
/*   Updated: 2024/03/08 11:01:49 by xiruwang         ###   ########.fr       */
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

	extract_redir(token, data);
	i = 0;
	data->cmd_num = count_pipe(*token) + 1;//eg. 2 pipe = 3 cmds
	data->cmd_list = NULL;
	while (i < data->cmd_num && *token) // loop thru to generate cmd_list
	{
		size = count_args(*token);
		new = creat_cmd(size, data);
		if (!new)
			free_exit("malloc error: new_cmd", data, EXIT_FAILURE);
		fill_cmd(token, new, data);//update token list
		append_cmd(&data->cmd_list, new);//append to cmd list
		i++;
	}
	return (data->cmd_list);
}

//loop thru the token list, find redirections and save it in data
//once found, remove from token list
void	extract_redir(t_token **head, t_data *data)
{
	t_token	*temp;
	t_token	*next_token;

	temp = *head;
	while (temp)
	{
		next_token = temp->next;
		if (temp->type == REDIR_IN || temp->type == REDIR_OUT)
		{
			if (!next_token || !next_token->value)//nothing behind redirection
				free_exit("syntax error near unexpected token", data, EXIT_FAILURE);
			if (temp->type == REDIR_IN)
			{
				if (data->in != NULL)//in case multiple <, only take the last one
					free(data->in);
				data->in = ft_strdup(next_token->value);
			}
			else if (temp->type == REDIR_OUT)
			{
				if (data->out != NULL)
					free(data->out);// 先释放之前的内存（如果有）
				data->out = ft_strdup(next_token->value);
			}
			else if (temp->type == APPEND)
			{
				if (data->out != NULL)
					free(data->out);// 先释放之前的内存（如果有）
				data->append = ft_strdup(next_token->value);
			}
			del_token(head, temp); // remove < >
			temp = next_token->next;//更新temp到下一个节点，因为next_token即将被删除
			del_token(head, next_token);//remove filename
		}
		else
			temp = temp->next;
	}
}

int		ft_bubiltin(char *s)
{
	if (ft_strncmp(s, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(s, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(s, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(s, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(s, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(s, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(s, "exit", 5) == 0)
		return (1);
	return (0);
}

void	fill_cmd(t_token **head, t_cmd *cmd, t_data *data)
{
	t_token	*temp;
	t_token	*next_token;

	temp = *head;
	if (!temp || temp->type == PIPE)//1st cmd cannot be PIPE
		free_exit("syntax error near unexpected token", data, STDERR);
	int i = 0;
	while (temp && temp->type != PIPE)
	{
		next_token = temp->next;
		if (temp->type != PIPE)
		{
			if (temp->type == WORD)//( i == 0 && temp->type == WORD)
			{
				cmd->s[i] = ft_strdup(temp->value);
				if (i == 0 && ft_bubiltin(cmd->s[i]))
					cmd->is_builtin = 1;
			}
			else if (temp->type == ARG || temp->type == S_QUO)
				cmd->s[i] = ft_strdup(temp->value);
			else if(temp->type == D_QUO)
				cmd->s[i] = handle_dollar(temp->value, data);
			// else if(temp->type == APPEND)
			// else if(temp->type == HEREDOC)
			del_token(head, temp);
			temp = next_token;
			i++;
		}
	}
	if (temp && temp->type == PIPE)//skip && delete the pipe
		del_token(head, temp);
}
