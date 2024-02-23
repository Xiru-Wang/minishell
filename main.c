/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/23 17:56:52 by xiruwang         ###   ########.fr       */
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
	t_data	*data;
	(void)av;
	char	s[] = "hi 42 | <>";

	//ft_memset(&data, 0, sizeof(data));
	data = (t_data *)malloc(sizeof(t_data));
	if (ac != 1)
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	init_data(data, env);
	printf("before strtrim\n");
	//minishell(&data);
	data->line = ft_strtrim(s, " \t\n\v\f\r");
	printf("before split\n");
	if (split_line(data->line, data->token_list) == 0)//data.token_list已经是**类型
	{
		free_token_list(data->token_list);//这是头指针
		printf("exit\n");
		free(data->line);
		free(data);
		return (2);
	}
	printf("print list\n");
	//print_list(*data->token_list);//这是1st node
	free_token_list(data->token_list);//这是头指针
	printf("exit\n");
	free(data->line);
	free(data);
	return (0);
}
