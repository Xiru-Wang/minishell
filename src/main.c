/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/27 18:47:58 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_last_signal;

static void	print_welcome_msg(int print_flag)
{
	if (print_flag)
	{
		printf("\
				╭────────────── 💃💃💃💃 ─────────────╮\n\
				│  Oh my shell, lucky its minishell   │\n\
				│             Jan && Xiru             │\n\
				╰────────────── 🍷🍷🍷🍷 ─────────────╯\n");
	}
}

static int	handle_input(t_data *data)
{
	if (!data->line)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (0);
	}
	if (data->line[0] == '\0')
	{
		free(data->line);
		return (-1);
	}
	add_history(data->line);
	return (1);
}

static int	process_line(t_data *data)
{
	split_line(data->line, &data->token_list, data);
	if (data->token_list == NULL)
	{
		free(data->line);
		return (-1);
	}
	if (generate_cmds(&data->token_list, &data->cmd_list, data) == 0)
	{
		init_signals_noint();
		executor(data->cmd_list, data);
	}
	return (0);
}

static int	minishell(t_data *data)
{
	int	input_status;

	while (1)
	{
		init_signals();
		data->line = readline("minishell>>");
		input_status = handle_input(data);
		if (input_status == 0)
			break ;
		if (input_status == -1)
			continue ;
		if (process_line(data) == -1)
			continue ;
		free(data->line);
		free_token_list(&data->token_list);
		free_cmd_list(&data->cmd_list);
	}
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	int		exit_code;

	g_last_signal = 0;
	exit_code = 0;
	if (ac != 1 || av[1])
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	data = (t_data *)malloc(sizeof(t_data));
	init_data(data, env);
	print_welcome_msg(1);
	minishell(data);
	exit_code = data->exit_code;
	free_data(data);
	return (exit_code);
}
