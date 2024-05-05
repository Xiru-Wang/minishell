/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/05 17:26:55 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void minishell(t_data *data)
{
	char	*s;

	while (1)
	{
		s = readline("minishell>>");
		if (!s)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (s[0] == '\0')
		{
			free(s);
			continue;
		}
		add_history(s);
		data->line = ft_strtrim(s, " \t\n\v\f\r");
		free(s);
		if (split_line(data->line, &data->token_list, data) == 0)
			free_exit("split_line", data, EXIT_FAILURE);
		data->cmd_list = generate_cmds(&data->token_list, data);
		print_cmd_list(data->cmd_list);
		//print_io_list(data->cmd_list);
		data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
		executor(data->cmd_list, data);
	}
	free_data(data);
	//TODO: how to exit the shell?
	//exit_shell();
	exit(EXIT_SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	if (ac != 1 || av[1])
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	data = (t_data *)malloc(sizeof(t_data));
	init_data(data, env);
	init_signal();
	minishell(data);
	return (0);
}
