/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/22 17:22:12 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int g_exit_code;

static void minishell(t_data *data)
{
	char	*s;

	while (1)
	{
		s = readline("minishell>>");
		if (s == NULL)
			break ;
		add_history(s);
		data->line = ft_strtrim(s, " \t\n\v\f\r");
		free(s);
		if (if_all_space(data->line) || !data->line)
			break ;
		// init_data();
		split_line(data->line, data);
	}
	printf("exit\n");
	exit_shell();
}


int	main(int ac, char **av, char **env)
{
	t_data	data;
	(void)av;
	(void)env;


	ft_memset(&data, 0, sizeof(data));
	if (ac != 1)
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	init_data(&data, env);
	minishell(&data);

	printf("exit\n");
	return (0);
}
