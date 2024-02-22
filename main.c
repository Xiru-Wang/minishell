/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/22 19:54:41 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int g_exit_code;

// static void minishell(t_data *data)
// {
// 	// while (1)
// 	// {
// 		//s = readline("minishell>>");
// 		// if (s == NULL)
// 		// 	break ;
// 		// add_history(s);
		
// 		data->line = ft_strtrim(s, " \t\n\v\f\r");
// 		//free(s);
// 		// if (if_all_space(data->line) || !data->line)
// 		// 	break ;
// 		// init_data();
// 		split_line(data->line, data);
// 	// }
// 	//exit_shell();
// }


int	main(int ac, char **av, char **env)
{
	t_data	data;
	(void)av;
	char	s[] = "	| hi 42";

	ft_memset(&data, 0, sizeof(data));
	if (ac != 1)
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	init_data(&data, env);
	printf("shell\n");
	//minishell(&data);
	data.line = ft_strtrim(s, " \t\n\v\f\r");
	split_line(data.line, &data);
	printf("print list\n");
	print_list(*(data.token_list));
	free_node(data.token_list);
	printf("exit\n");
	free(data.line);
	return (0);
}
