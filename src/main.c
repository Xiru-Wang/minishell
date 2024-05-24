/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/24 12:13:46 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_last_signal;

/* static void	print_welcome_msg(void) */
/* { */
/*     printf("\ */
/*             ╭────────────── 💃💃💃💃 ─────────────╮\n\ */
/*             │  Oh my shell, lucky its minishell   │\n\ */
/*             │             Jan && Xiru             │\n\ */
/*             ╰────────────── 🍷🍷🍷🍷 ─────────────╯\n"); */
/* } */

static void	free_mini(t_data *data)
{
	free(data->line);
	free_token_list(&data->token_list);
	free_cmd_list(&data->cmd_list);
}

static int	minishell(t_data *data)
{
	while (1)
	{
		init_signals();
		data->line = readline("minishell>>");
		if (!data->line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (data->line[0] == '\0')
		{
			free(data->line);
			continue ;
		}
		add_history(data->line);
		split_line(data->line, &data->token_list, data);
		data->cmd_list = generate_cmds(&data->token_list, data);
		//print_cmd_list(data->cmd_list);//debug
		if (data->exit_code != 2 && data->cmd_list)
		{
			init_signals_noint();
			executor(data->cmd_list, data);
		}
		free_mini(data);
	}
	return (EXIT_SUCCESS);
}

//heredoc_conrol + c: exit minishell!!
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
	/* print_welcome_msg(); */
	minishell(data);
	exit_code = data->exit_code;
	free_data(data);
	return (exit_code);
}
