
#include "../../includes/minishell.h"

int	call_pwd(t_cmd *cmd)
{
	if (cmd->s[1])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(cmd->data->pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
