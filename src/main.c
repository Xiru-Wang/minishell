/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/04/27 18:17:25 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void minishell(t_data *data)
{
	char	*s;

	while (1)
	{
		s = readline("minishell>>");
		if (s == NULL)
		{
			printf("exit\n");
			break ;
		}
		add_history(s);
		data->line = ft_strtrim(s, " \t\n\v\f\r");
		free(s);
		if (split_line(data->line, &data->token_list, data) == 0)
			free_exit("split_line", data, EXIT_FAILURE);
		printf("after split line\n");
		//TODO: make tests for this
		data->cmd_list = generate_cmds(&data->token_list, data);
		//FIX:
		printf("after generate cmds\n");
		print_cmd_list(data->cmd_list);
		data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
		if (data->cmd_list)
		{
			printf("BEFORE EXECUTOR\n");
			executor(data->cmd_list, data);
		}
			
	}
	//exit_shell();
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	if (ac != 1 || av[1])
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	init_signals();
	data = (t_data *)malloc(sizeof(t_data));
	init_data(data, env);
	/* welcome_msg(); */
	minishell(data);
	return (0);
}
