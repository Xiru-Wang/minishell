/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:49 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/11 09:17:11 by xiruwang         ###   ########.fr       */
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

//if here_doc got interrupted??

static int    create_hd(t_cmd *cmd, int expand_sign)
{
    int     fd, status;
    int     i;
    char    *line;
    char    *new;

    fd = open(cmd->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
    g_last_signal = 0;
    if (fork() == 0)
    {
        signal(SIGINT, SIG_DFL);  // set SIGINT as default in child
        while (1)
        {
            i = 0;
            line = readline("heredoc>");
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
            i++;
        }
        if (!line)
            printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", i, cmd->delimiter);
        free(line);
        close(fd);
        exit(0);
    }
    else
    {
        wait(&status);
        close(fd);
        signal(SIGINT, signal_handler);  // reset SIGINT in parent
        g_last_signal = 1;// exit heredoc, set signal to be true
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            unlink(cmd->hdfile);
            return (1);
        }
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        else
            return (1);
    }
}

// static int	create_hd(t_cmd *cmd, int expand_sign)
// {
// 	int		fd;
// 	int		i;
// 	char	*line;
// 	char	*new;

// 	fd = open(cmd->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
// 	if (fork() == 0)
// 	{
// 		cmd->data->in_heredoc = 1;
// 		init_heredoc_sig();
// 	}

// 	while (1)
// 	{
// 		line = readline("heredoc>");
// 		i = 1;
// 		if (ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
// 			break;
// 		if (expand_sign == 0 && check_valid_dollar(line))
// 		{
// 			new = expand_simple(line, cmd->data->env, cmd->data);
// 			ft_putendl_fd(new, fd);
// 			free(new);
// 		}
// 		else
// 			ft_putendl_fd(line, fd);
// 		free(line);
// 		line = readline("heredoc>");
// 		i++;
// 	}
// 	if (!line)
// 		printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", i, cmd->delimiter);
// 	free(line);
// 	close(fd);
// 	return (1);
// }

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
