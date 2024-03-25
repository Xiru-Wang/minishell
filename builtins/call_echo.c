#include "../includes/minishell.h"

//echo
//with -n: no new line
//without -n: new line
//s[1] has to be -n (with a random number of n's) eg. -nnnn->works
//eg> echo -nn -nnnn2 -n "hi " "dad" "blala    lala"
//-nnnn2 -n hi  dad blala    lala%

void call_echo(t_cmd *cmd)
{
	int	i;
	int	k;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd->s[i])
	{
		k = 1;
		if (cmd->s[i][0] == '-' && cmd->s[i][1] == 'n')//first arg must be -n
		{
			while(cmd->s[i][k] == 'n')//-nnnnn skip rest n
				k++;
			if (cmd->s[i][k] == '\0')//-n2 wrong
				flag = 1;
			else
				break ;
		}
		i++;
	}
	while (cmd->s[i])
	{
		if (!ft_strncmp(cmd->s[i], "$?", 3))
			ft_putnbr_fd(g_last_exit_code, STDOUT_FILENO);
		else
		{
			ft_putstr_fd(cmd->s[i], STDOUT_FILENO); // 去除掉引号
		}

		if (cmd->s[i])//在输出每个参数之间添加空格,但避免在最后一个参数后面添加多余的空格
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (flag == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
