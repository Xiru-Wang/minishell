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
	while (cmd->s[i])
	{
		temp = ft_strjoin(cmd->s[i], "=");
		k = 0;
		while (data->env[k])
		{
			if (ft_strncmp(data->env[k], temp, ft_strlen(temp)) == 0)
			{
				data->env = reset_env(data, data->env, k);
				break;
			}
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
	int		j;
	char	**new_env;
	(void) data;

	i = 0;
	j = 0;
	while (env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (env[i])
	{
		if (i != k)
		{
			new_env[i] = env[i];
			j++;
		}
		else
			free(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free(env);
	return (new_env);
}
