/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/09 12:39:52 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
static int	remove_hd_quotes(t_cmd *cmd);
static char	*create_hd_name(void);
static int	create_hd(t_cmd *cmd, int eof_quote);

void	check_hd(t_cmd *cmd)
{
	t_io	*temp;
	int		quote;

	if (!cmd->io_list)
		return ;
	temp = cmd->io_list;
	while (temp)
	{
		if (temp->type == HEREDOC)
		{
			if (cmd->hdfile)
				free(cmd->hdfile);
			cmd->hdfile = create_hd_name();
			quote = remove_hd_quotes(cmd);
			create_hd(cmd, quote);
		}
		temp = temp->next;
	}
}

/*
static int readline_event_hook_heredoc(void) {
    if (last_received_signal == SIGINT) {
        rl_replace_line("", 0);
        rl_done = 1;
        write(STDOUT_FILENO, "\n", 1);
        close(rl_instream->_fileno);
        last_received_signal = 0;
        return (1);
    }
    return (0);
}
static int    create_hd(t_cmd *cmd, int expand_sign)
{
    int     fd;
    int     i;
    char    *line;
    char    *new;

    fd = open(cmd->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
    
    line = NULL;
    i = 1;
    last_received_signal = 0;  // Initialize last_received_signal to 0
    while (1)
    {
        rl_event_hook = readline_event_hook_heredoc;
        line = readline("heredoc>");
        if (rl_event_hook == readline_event_hook_heredoc && last_received_signal == SIGINT)
            break;
        rl_event_hook = NULL;

        if (line == NULL || ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
            break;
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
    if (line == NULL && last_received_signal != SIGINT)
        printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", i, cmd->delimiter);
    free(line);
    close(fd);

    if (last_received_signal == SIGINT) {
        unlink(cmd->hdfile);
        last_received_signal = 0;
        return (0);
    }
    return (1);
}
*/

//if here_doc got interrupted??
static int	create_hd(t_cmd *cmd, int expand_sign)
{
	int		fd;
	int		i;
	char	*line;
	char	*new;

	fd = open(cmd->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline("heredoc>");
	i = 1;
	while (line)
	{
		if (ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
			break;
		if (expand_sign == 0 && check_valid_dollar(line))
		{
			new = expand_simple(line, cmd->data->env, cmd->data);
			ft_putendl_fd(new, fd);
			free(new);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
		line = readline("heredoc>");
		i++;
	}
	if (!line)
		printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", i, cmd->delimiter);
	free(line);
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
