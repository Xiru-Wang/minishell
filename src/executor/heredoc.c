/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/19 18:54:48 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	remove_hd_quotes(t_io *io);
static char	*create_hd_name(void);
static int	create_hd(t_cmd *cmd, t_io *io, int eof_quote);

int	check_hd(t_cmd *cmd)
{
	t_io	*temp;
	int		quote;

	if (!cmd->io_list)
		return (0);
	temp = cmd->io_list;
	while (temp)
	{
		if (temp->type == HEREDOC)
		{
			temp->hdfile = create_hd_name();
			quote = remove_hd_quotes(temp);
			if (create_hd(cmd, temp, quote) == 130)
				return (130);
		}
		if (!temp->next)
			break ;
		temp = temp->next;
	}
	return (0);
}


//0600: The file owner has read and write permissions (rw-------). No one else can read or write to the file.
//0644: The file owner has read and write permissions, but others can only read the file (rw-r--r--).

static int	create_hd(t_cmd *cmd, t_io *io, int expand_sign)
{
	int		fd;
	int		i;
	char	*line;
	char	*new;

	fd = open(io->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0600);
	setup_signals_hd();
	i = 0;
	while (1)
	{
		line = readline("heredoc>");
		i++;
		if (!line)
			break ;
		if (g_last_signal == 2)
		{
			free(line);
			close(fd);
			reset_signals_hd();
			return (EXIT_SIGINT);
		}
		if (ft_strncmp(line, io->eof, ft_strlen(io->eof) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (expand_sign == 0 && check_valid_dollar(line))
		{
			new = expand_simple(line, cmd->data->env, cmd->data);
			ft_putendl_fd(new, fd);
			free(new);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
	}
	if (!line)
		printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", i, io->eof);
	close(fd);
	reset_signals_hd();
	return (EXIT_SUCCESS);
}

static int	remove_hd_quotes(t_io *io)
{
	char	*s;

	s = io->eof;
	if ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"')
		|| (s[0] == '\'' && s[ft_strlen(s) - 1] == '\''))
	{
		io->eof = ft_substr(s, 1, ft_strlen(s) - 2);
		free(s);
		return (EXIT_FAILURE);
	}
	else
		return (EXIT_SUCCESS);
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