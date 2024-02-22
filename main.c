/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/21 22:23:42 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int g_exit_code;

static void minishell(t_session *session)
{
	while (1)
	{
		session->line = readline("minishell>>");
		if (session->line == NULL)
			break ;
		printf("before split_line");//debug
		session->tokens = split_line(session->line);

		free_double_ptr(session->tokens);
		//lexer(session);
	}
	printf("exit\n");
	exit_shell();
}


int	main(int ac, char **av, char **env)
{
	t_session	session;
	(void)av;
	(void)env;


	ft_memset(&session, 0, sizeof(t_session));
	if (ac != 1)
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	minishell(&session);

	printf("exit\n");
	return (0);
}
