
#include "../../includes/minishell.h"

int	call_pwd(t_cmd *cmd)
{
	ft_putendl_fd(cmd->data->pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
