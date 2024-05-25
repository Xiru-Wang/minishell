/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/25 19:01:11 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_hd_quotes(t_io *io);
static char	*create_hd_name(void);
static int	create_hd(t_cmd *cmd, t_io *io);

int	check_hd(t_cmd *cmd)
{
	t_io	*temp;

	if (!cmd->io_list)
		return (0);
	temp = cmd->io_list;
	while (temp)
	{
		if (temp->type == HEREDOC)
		{
			temp->hdfile = create_hd_name();
			remove_hd_quotes(temp);
			if (create_hd(cmd, temp) == EXIT_SIGINT)
				return (EXIT_SIGINT);
		}
		if (!temp->next)
			break ;
		temp = temp->next;
	}
	return (0);
}

static char	*read_heredoc(int fd, t_cmd *cmd, t_io *io, int *i)
{
	char	*line;
	char	*new;

	line = NULL;
	while (1)
	{
		line = readline("heredoc>");
		(*i)++;
		if (!line || g_last_signal == 2 || \
			ft_strncmp(line, io->eof, ft_strlen(io->eof) + 1) == 0)
			break ;
		if (io->eof_quo == 0 && check_valid_dollar(line))
		{
			new = expand_simple(line, cmd->data);
			ft_putendl_fd(new, fd);
			free(new);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
	}
	return (line);
}

//0600: The file owner has read and write permissions (rw-------).
//0644: Others can only also read the file (rw-r--r--).
static int	create_hd(t_cmd *cmd, t_io *io)
{
	int		fd;
	int		i;
	char	*line;

	i = 0;
	line = NULL;
	fd = open(io->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0600);
	setup_signals_hd();
	line = read_heredoc(fd, cmd, io, &i);
	if (g_last_signal == 2)
	{
		free(line);
		close(fd);
		reset_signals_hd();
		return (EXIT_SIGINT);
	}
	if (!line)
		printf("minishell: warning: here-document at line %d delimited "
			"by end-of-file (wanted `%s')\n", i, io->eof);
	free(line);
	close(fd);
	reset_signals_hd();
	return (EXIT_SUCCESS);
}

//"EOF" or 'EOF': Do not expand $var in heredoc
static void	remove_hd_quotes(t_io *io)
{
	char	*s;

	s = io->eof;
	if ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"')
		|| (s[0] == '\'' && s[ft_strlen(s) - 1] == '\''))
	{
		io->eof = ft_substr(s, 1, ft_strlen(s) - 2);
		free(s);
		io->eof_quo = 1;
	}
}

//to avoid heredoc conflict(eg. cmd <<eof <<EOF)
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
