/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/19 19:14:40 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
static void	print_welcome_msg(void);

void	minishell(t_data *data)
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
		if (split_line(data->line, &data->token_list, data) == 0)
			free_exit("split_line", data, EXIT_FAILURE);
		data->cmd_list = generate_cmds(&data->token_list, data);
		if (data->cmd_list)
		{
			init_signals_noint();
			if (executor(data->cmd_list, data) == 1)  // if interrupted?
				continue;  //do we need?
		}
		free(data->line);
		free_token_list(&data->token_list);
		free_cmd_list(&data->cmd_list);
	}
	exit(EXIT_SUCCESS);
}

//heredoc_conrol + c: exit minishell!!
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
	print_welcome_msg();
	minishell(data);
	free_data(data);
	//free(data);
	return (0);
}

static void	print_welcome_msg(void)
{
	printf("\
			╭────────────── 💃💃💃💃 ─────────────╮\n\
			│  Oh my shell, lucky its minishell   │\n\
			│             Jan && Xiru             │\n\
			╰────────────── 🍷🍷🍷🍷 ─────────────╯\n");
}
