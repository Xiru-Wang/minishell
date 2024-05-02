/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/02 20:16:58 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_exit_code = 0;

void minishell(t_data *data)
{
	char	*s;

	while (1)
	{
		s = readline("minishell>>");
		if (s == NULL)
		{
			write(STDERR_FILENO, "exit\n", 5);
			break ;
		}
		add_history(s);
		data->line = ft_strtrim(s, " \t\n\v\f\r");
		free(s);
		if (split_line(data->line, &data->token_list, data) == 0)
			free_exit("split_line", data, EXIT_FAILURE);
		data->cmd_list = generate_cmds(&data->token_list, data);
		print_cmd_list(data->cmd_list);
		data->pid = ft_calloc(data->cmd_num, sizeof(pid_t));
		if (data->cmd_list)
			executor(data->cmd_list, data);
		/* if (g_exit_code == EXIT_SUCCESS || g_exit_code == EXIT_FAILURE) */
		/*     break; */
	}
	// free_data(data);
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
	sig_init();
	/* welcome_msg(); */
	minishell(data);
	return (0);
}
