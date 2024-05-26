/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:07 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 10:13:19 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_cd(t_data *data, t_cmd *cmd)
{
	char	*path;
	int		ret;

	path = resolve_cd_path(data, cmd);
	if (path == NULL)
	{
		if (cmd->s[2] || (!cmd->s[1] && !find_env_var(data, "HOME")))
			return (EXIT_FAILURE);
		if (cmd->s[1] && ft_strncmp(cmd->s[1], "", 1) == 0)
			return (EXIT_SUCCESS);
		return (EXIT_FAILURE);
	}
	ret = change_directory(data, path);
	if (ret == -1)
		print_cd_error(path);
	else
	{
		if (cmd->s[1] && ft_strncmp(cmd->s[1], "-", 1) == 0)
			ft_putendl_fd(data->pwd, STDOUT_FILENO);
		update_pwd_variables(data);
	}
	return (ret == -1);
}

char	*find_env_var(t_data *data, const char *var_name)
{
	int		i;
	size_t	var_len;

	var_len = ft_strlen(var_name);
	i = 0;
	while (data->env[i] != NULL)
	{
		if (ft_strncmp(data->env[i], var_name, var_len) == 0 && \
				data->env[i][var_len] == '=')
			return (data->env[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

char	*handle_cd_oldpwd(t_data *data)
{
	if (data->old_pwd == NULL)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
		return (NULL);
	}
	return (data->old_pwd);
}

int	change_directory(t_data *data, char *path)
{
	int		ret;
	char	*tmp;

	ret = chdir(path);
	if (ret == -1)
		return (-1);
	if (data->old_pwd != NULL)
		free(data->old_pwd);
	data->old_pwd = ft_strdup(data->pwd);
	if (data->pwd)
		free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
	{
		ft_putendl_fd("cd: error retrieving current directory: getcwd: "
			"cannot access parent directories: No such file or directory",
			STDERR_FILENO);
		tmp = ft_strjoin(data->old_pwd, "/.");
		data->pwd = tmp;
	}
	return (EXIT_SUCCESS);
}
