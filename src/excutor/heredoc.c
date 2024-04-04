/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/03 21:15:36 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
static int	remove_hd_quotes(t_cmd *cmd);
static char	*create_hd_name(void);
static int	create_hd(t_cmd *cmd, int eof_quote);

int	check_hd(t_cmd *cmd)
{
	t_io	*temp;
	//int		sl;
	int		quote;

	temp = cmd->io_list;
	//sl = EXIT_SUCCESS;//??
	while (temp)
	{
		if (temp->type == HEREDOC)
		{
			if (cmd->hdfile)
				free(cmd->hdfile);
			cmd->hdfile = create_hd_name();
			quote = remove_hd_quotes(cmd); // update EOF
			create_hd(cmd, quote);
			/*
			TODO
			if (sl)
			{
				g_global.error_num = 1;
				return (reset_tools(tools));
			}
			*/
		}
		temp = temp->next;
	}
	return (1);
}

//1. write content to heredoc from keyboard
//2. later read data from heredoc
static int	check_quotes(char *line)
{
	while(*line)
	{
		if (*line == '\'' || *line == '\"')
			return (1);
		line ++;
	}
	//what if unclosed quo??
	return (0);
}

static int	create_hd(t_cmd *cmd, int expand_sign)
{
	int		fd;
	char	*line;
	char	*new;

	fd = open(cmd->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline("heredoc>");
	while (line)//TODO
	{
		if (ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
			break;
		if (expand_sign == 0)
		{
			if (check_quotes(line))
				new = check_dollar_quo(line, cmd->data, QUO);
			else
				new = check_dollar_quo(line, cmd->data, WORD);
			ft_putendl_fd(new, fd);//write to temp file
			free(new);
		}
		else
			ft_putendl_fd(line, fd);//write to temp file
		free(line);
		line = readline("heredoc>");
	}
	free(line);
	// if (g_global.stop_heredoc || !line)//TODO
	// 	return (EXIT_FAILURE);
	close(fd);
	return (1);
}

static int	remove_hd_quotes(t_cmd *cmd)
{
	char	*s;

	s = cmd->delimiter;
	if ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"')
		|| (s[0] == '\'' && s[ft_strlen(s) - 1] == '\''))
	{
		cmd->delimiter = ft_substr(s, 1, ft_strlen(s) - 2);
		free(s);
		return (1);
	}
	else
		return (0);
}

//to ensure temp here_doc's filename does not have conflict
static char	*create_hd_name(void)
{
	static int	i;
	char		*num;
	char		*name;

	i = 0;
	num = ft_itoa(i);
	i++;
	name = ft_strjoin("/tmp/minihd_", num);
	free(num);
	return (name);
}