/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:07 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 10:13:30 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

void	update_pwd_variables(t_data *data)
{
	if (data->pwd && ft_strncmp(data->pwd, "", 1) != 0)
		update_env_var(data, "PWD", data->pwd);
	if (data->old_pwd && ft_strncmp(data->old_pwd, "", 1) != 0)
		update_env_var(data, "OLDPWD", data->old_pwd);
}

void	update_env_var( \
		t_data *data, const char *var_name, const char *new_value)
{
	int		i;
	size_t	var_len;
	char	*new_var;
	char	*new_value_copy;

	if (!var_name || !new_value)
		return ;
	var_len = ft_strlen(var_name);
	i = 0;
	while (data->env[i] != NULL)
	{
		if (ft_strncmp(data->env[i], var_name, var_len) == 0 && \
				data->env[i][var_len] == '=')
		{
			free(data->env[i]);
			new_var = ft_strjoin(var_name, "=");
			new_value_copy = ft_strdup(new_value);
			data->env[i] = ft_strjoin(new_var, new_value_copy);
			free(new_var);
			free(new_value_copy);
			return ;
		}
		i++;
	}
	add_new_env_var(data, var_name, new_value, i);
}

void	add_new_env_var( \
		t_data *data, const char *var_name, const char *new_value, int i)
{
	char	*new_var;
	char	*env_var;
	char	**new_env;

	new_var = ft_strjoin(var_name, "=");
	env_var = ft_strjoin(new_var, new_value);
	free(new_var);
	new_env = malloc(sizeof(char *) * (i + 2));
	if (new_env == NULL)
	{
		free(env_var);
		return ;
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
