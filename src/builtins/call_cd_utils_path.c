/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd_utils_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:52:07 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/26 10:13:43 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_home_directory(t_data *data)
{
	char	*home;

	home = find_env_var(data, "HOME");
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	return (home);
}

static char	*get_oldpwd_directory(t_data *data)
{
	char	*oldpwd;

	oldpwd = handle_cd_oldpwd(data);
	if (!oldpwd)
		return (NULL);
	return (oldpwd);
}

static int	check_too_many_arguments(t_cmd *cmd)
{
	if (cmd->s[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

char	*resolve_cd_path(t_data *data, t_cmd *cmd)
{
	if (!cmd->s[1] || (cmd->s[1] && ft_strncmp(cmd->s[1], "~", 2) == 0))
		return (get_home_directory(data));
	if (check_too_many_arguments(cmd))
		return (NULL);
	if (cmd->s[1] && ft_strncmp(cmd->s[1], "", 1) == 0)
		return (NULL);
	if (ft_strncmp(cmd->s[1], "-", 2) == 0)
		return (get_oldpwd_directory(data));
	return (cmd->s[1]);
}
