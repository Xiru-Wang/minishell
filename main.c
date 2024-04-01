/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/04/01 19:45:23 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void minishell(t_data *data)
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
		init_data(data, data->env);
		if (split_line(data->line, &data->token_list, data) == 0)
			free_exit("split_line", data, EXIT_FAILURE);
 		data->cmd_list = generate_cmd(&data->token_list, data);
		executor(data->cmd_list);
	}
	//exit_shell();
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	(void)av;
	char	s[] = "ls -l< infile| wc -l | echo \"hello\" > outfile";
	if (ac != 1)
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	//setup_sig();
	data = (t_data *)malloc(sizeof(t_data));
	init_data(data, env);
	//minishell(&data);
	return (0);
}
