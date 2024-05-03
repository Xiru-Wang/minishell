#include "../../includes/minishell.h"

//display nothing if no args for the cmd

static char	**reset_env(t_data *data, char **env, int k);

int	call_unset(t_cmd *cmd, t_data *data)
{
	int		i;
	int		k;
	char	*temp;

	if (!cmd->s[1])
		return (EXIT_SUCCESS);
	i = 1;
	temp = NULL;
	while (cmd->s[i])
	{
		temp = ft_strjoin(cmd->s[i], "=");
		k = 0;
		while (data->env[k])
		{
			if (ft_strncmp(data->env[k], temp, ft_strlen(temp) == 0))
				data->env = reset_env(data, data->env, k);
			k++;
		}
		free(temp);
		i++;
	}
	return (EXIT_SUCCESS);
}

static char	**reset_env(t_data *data, char **env, int k)
{
	int		i;
	char	**new_env;
	(void) data;

	i = 0;
	while (env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	new_env[i - 1] = NULL;
	i = 0;
	while (env[i])
	{
		if (i == k)
			i++;
		new_env[i] = env[i];
		i++;
	}
	free_double_ptr(env);
	return (new_env);
}
