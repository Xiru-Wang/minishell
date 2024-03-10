/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiwang <xiwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/03/10 16:04:51 by xiwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int g_exit_code;

// static void minishell(t_data *data)
// {
// 	char	*s;

// 	while (1)
// 	{
// 		s = readline("minishell>>");
// 		if (s == NULL)
// 			break ;
// 		add_history(s);
// 		data->line = ft_strtrim(s, " \t\n\v\f\r");
// 		free(s);
// 		init_data(data, data->env);
// 		if (split_line(data->line, &data->token_list, data) == 0)
// 			free_exit("split_line", data, EXIT_FAILURE);
// 		data->cmd_list = generate_cmd(&data->token_list, data);
//		if (cmd_list)
// 			executor();
// 	}
// 	//exit_shell();
// }

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	(void)av;
	char	s[] = "ls -l< infile| wc -l | echo \"hello\" > outfile";

	data = (t_data *)malloc(sizeof(t_data));
	if (ac != 1)
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	init_data(data, env);
	//minishell(&data);
	data->line = ft_strtrim(s, " \t\n\v\f\r");
	//lexer
	if (split_line(data->line, &data->token_list, data) == 0)
		free_exit("exit: split_line == 0", data, EXIT_FAILURE);
	print_list(data->token_list);//debug
	//parser
	data->cmd_list = generate_cmd(&data->token_list, data);
	print_cmd_list(data->cmd_list);//debug
	if (data->infile)//debug
		printf("%s\n", data->infile);
	if (data->outfile)//debug
		printf("%s\n", data->outfile);
	free_exit("success", data, EXIT_FAILURE);
	return (0);
}
