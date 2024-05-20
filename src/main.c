/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/20 14:01:25 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

/* static void	print_welcome_msg(void); */
int	minishell(t_data *data)
{
	while (1)
	{
		init_signals();
		data->line = readline("minishell>>");
		if (!data->line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (data->line[0] == '\0')
		{
			free(data->line);
			continue;
		}
		add_history(data->line);
		split_line(data->line, &data->token_list, data);
		data->cmd_list = generate_cmds(&data->token_list, data);
		if (data->exit_code != 2 && data->cmd_list)
		{
			init_signals_noint();
			executor(data->cmd_list, data);
		}
		free(data->line);
		free_token_list(&data->token_list);
		free_cmd_list(&data->cmd_list);
	}
	return (EXIT_SUCCESS);
}

//heredoc_conrol + c: exit minishell!!
int	main(int ac, char **av, char **env)
{
	t_data	*data;
	int last_exit_code;

	last_exit_code = 0;
	if (ac != 1 || av[1])
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	data = (t_data *)malloc(sizeof(t_data));
	init_data(data, env);
	/* print_welcome_msg(); */
	minishell(data);
	last_exit_code = data->exit_code;
	free_data(data);
	return (last_exit_code);
}

/* static void	print_welcome_msg(void) */
/* { */
/*     printf("\ */
/*             ╭────────────── 💃💃💃💃 ─────────────╮\n\ */
/*             │  Oh my shell, lucky its minishell   │\n\ */
/*             │             Jan && Xiru             │\n\ */
/*             ╰────────────── 🍷🍷🍷🍷 ─────────────╯\n"); */
/* } */
