/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/19 00:17:05 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	remove_hd_quotes(t_cmd *cmd);
static char	*create_hd_name(void);
static int	create_hd(t_cmd *cmd, int eof_quote);
static int	reset_hd_file(int *fd, const char *filename);

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
			if (cmd->hdfile)
				free(cmd->hdfile);
			cmd->hdfile = create_hd_name();
			quote = remove_hd_quotes(cmd);
			//if (create_hd(cmd, quote) != 0)//interrpted by signal
			if (create_hd(cmd, quote) == 130)
				return (130);
		}
		if (!temp->next)
			break ;
		temp = temp->next;
	}
	return (0);
}

static int	create_hd(t_cmd *cmd, int expand_sign)
{
	int		fd;
	int		i;
	char	*line;
	char	*new;

	fd = open(cmd->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	setup_signals_hd();
	while (1)
	{
		line = readline("heredoc>");
		i = 1;
		if (line == NULL || g_last_signal == 2)
		{
			if (g_last_signal == 2)
				reset_hd_file(&fd, cmd->hdfile);
			free(line);
			close(fd);//!!ADDED
			reset_signals_hd();
			return (EXIT_SIGINT);//return 2?? 128 + 2 = 130
		}
		if (ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
		{
			close(fd);
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
		i++;
	}
	if (!line)
		printf("minishell: warning: here-document at line %d \
				delimited by end-of-file (wanted `%s')\n", i, cmd->delimiter);
	close(fd);
	reset_signals_hd();
	return (EXIT_SUCCESS);
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

static int	reset_hd_file(int *fd, const char *filename)
{
	close(*fd);
	*fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (*fd == -1)
		errno = EINTR;
	close(*fd);
	return (EXIT_SUCCESS);
}

// static int reset_hd_file(int *fd, const char *filename)
// {
//     close(*fd);
//     *fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
//     if (*fd == -1)
//     {
//         perror("reset_hd_file");
//         return (EXIT_FAILURE);
//     }
//     if (close(*fd) == -1)
//     {
//         perror("reset_hd_file");
//         return (EXIT_FAILURE);
//     }
//     return (EXIT_SUCCESS);
// }
