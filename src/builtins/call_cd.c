/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:07 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/08 20:31:21 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>

int call_cd(t_data *data, t_cmd *cmd)
{
	char *path;
	int ret;

	if (cmd->s[1] == NULL || ft_strncmp(cmd->s[1], "~", 1) == 0)
		path = find_env_var(data, "HOME");
	else if (ft_strncmp(cmd->s[1], "-", 1) == 0)
	{
		path = handle_cd_oldpwd(data);
		if (path == NULL)
			return (1);
	}
	else
		path = cmd->s[1];

	ret = change_directory(data, path);
	if (ret == -1)
		print_cd_error(path);
	else
		update_pwd_variables(data);

	return (ret == -1);
}

char *find_env_var(t_data *data, const char *var_name)
{
	int i;
	size_t var_len;

	var_len = ft_strlen(var_name);
	i = 0;

	while (data->env[i] != NULL)
	{
		if (ft_strncmp(data->env[i], var_name, var_len) == 0 && data->env[i][var_len] == '=')
			return (data->env[i] + var_len + 1);
		i++;
	}

	return (NULL);
}

char *handle_cd_oldpwd(t_data *data)
{
	if (data->old_pwd == NULL)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
		return (NULL);
	}
	ft_putendl_fd(data->old_pwd, STDOUT_FILENO);
	return (data->old_pwd);
}

int change_directory(t_data *data, char *path)
{
    int ret;

    ret = chdir(path);
    if (ret == -1)
        return (-1);

    if (data->old_pwd != NULL)
        free(data->old_pwd);
    data->old_pwd = ft_strdup(data->pwd);
    //free(data->pwd);
	if (data->pwd)
		free(data->pwd);
    data->pwd = getcwd(NULL, 0);

    return (0);
}

void print_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

void update_pwd_variables(t_data *data)
{
    update_env_var(data, "PWD", data->pwd);

    if (find_env_var(data, "OLDPWD") != NULL)
        update_env_var(data, "OLDPWD", data->old_pwd);
}

void update_env_var(t_data *data, const char *var_name, const char *new_value)
{
    int i;
    size_t var_len;
    char *new_var;
    char *new_value_copy;

    var_len = ft_strlen(var_name);
    i = 0;

    while (data->env[i] != NULL)
    {
        if (ft_strncmp(data->env[i], var_name, var_len) == 0 && data->env[i][var_len] == '=')
        {
            free(data->env[i]);
            new_var = ft_strjoin(var_name, "=");
            new_value_copy = ft_strdup(new_value);
            data->env[i] = ft_strjoin(new_var, new_value_copy);
            free(new_var);
            free(new_value_copy);
            return;
        }
        i++;
    }

    add_new_env_var(data, var_name, new_value, i);
}

void add_new_env_var(t_data *data, const char *var_name, const char *new_value, int i)
{
    char *new_var;
    char *env_var;
    char **new_env;

    new_var = ft_strjoin(var_name, "=");
    env_var = ft_strjoin(new_var, new_value);
    free(new_var);
    new_env = malloc(sizeof(char *) * (i + 2));
    if (new_env == NULL)
    {
        free(env_var);
        return;
    }
    i = 0;
    while (data->env[i] != NULL)
    {
        new_env[i] = data->env[i];
        i++;
    }
    new_env[i] = env_var;
    new_env[i + 1] = NULL;
    free(data->env);
    data->env = new_env;
}
