/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 15:22:28 by xiruwang          #+#    #+#             */
/*   Updated: 2024/04/01 18:22:34 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*find_path(char *s, char **env);

int	call_cmd(t_data *data, t_cmd *cmd)
{
	char	*path;

	path = find_path(cmd->s[0], data->env);
	if (!path)
	{
		write(STDERR_FILENO, cmd->s[0], ft_strlen(cmd->s[0]));
		write(STDERR_FILENO, ":command not found\n", 19);
		exit(127);
	}
	execve(path, cmd->s, data->env);
	free(path);
	free_exit("execve", data, STDERR_FILENO);
	return (0);
}

static char	*find_path(char *s, char **env)
{
	char	**paths;
	char	*temp;
	char	*cmd_path;
	int		i;

	i = 0;
	while (ft_strnstr(env[i], "PATH=", 5) == NULL)
		i++;
	paths = ft_split(env[i] + 5, ':');
	i = -1;
	while(paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(temp, s);
		free(temp);
		if (access(cmd_path, F_OK | X_OK) == 0)//existence || execute/search
		{
			free(paths);
			return (cmd_path);//success
		}
		free(cmd_path);
	}
	free_double_ptr(paths);
	return (NULL);
}
