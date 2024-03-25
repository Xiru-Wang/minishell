#include "includes/minishell.h"

int hd_quotes(t_cmd *cmd)
{
	char	*s;
	int		quotes;

	s = cmd->delimiter;
	if ((s[0] == '\"' && s[ft_strlen(s) - 1] == '\"')
		|| (s[0] == '\'' && s[ft_strlen(s) - 1] == '\''))
	{
		cmd->delimiter = ft_substr(s, 1, ft_strlen(s) - 2);//length..?
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
	char		name;

	i = 0;
	num = ft_itoa(i);
	i++;
	name = ft_strjoin("/tmp/minihd_", num);
	free(num);
	return (name);
}

//write content to heredoc, later get data from heredoc
void	create_hd(t_cmd *cmd, t_data *data)
{
	int		quote;
	int		fd;//temp
	char	*line;

	if (cmd->infile)//if multiple infile, last one take place
		free(cmd->infile);
	cmd->hdfile = create_hd_name();
	fd = open(cmd->hdfile, O_CREAT | O_RDWR | O_TRUNC, 0644);//write to
	quote = hd_quotes(cmd); // update EOF
	line = readline("heredoc>");
	while(line && global_signal)
	{
		if (!cmd->delimiter && signal == control + d)
			break ;
		if (ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
			break;
		if (quote == 0)
			line = handle_dollar(line, data);//expand $
		ft_putendl_fd(line, fd);//write to temp file
		free(line);
		line = readline("heredoc>");
	}
	free(line);
	close(fd);
}
