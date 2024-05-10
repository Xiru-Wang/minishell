/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/05/10 16:19:06 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
static void	print_welcome_msg(void);

void	minishell(t_data *data)
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
		if (data->cmd_list)
			executor(data->cmd_list, data);
	}
	free_data(data);
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
	print_welcome_msg();
	minishell(data);
	return (0);
}

static void	print_welcome_msg(void)
{
	// if (isatty(STDIN_FILENO))
	// 	printf("%s", "\e[2J\e[1;1H");
	printf("\
            ╭────────────── 💃💃💃💃 ─────────────╮\n\
            │  Oh my shell, lucky its minishell   │\n\
            │             Jan && Xiru             │\n\
            ╰────────────── 🍷🍷🍷🍷 ─────────────╯\n");
}
