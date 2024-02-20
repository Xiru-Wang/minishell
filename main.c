/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/14 20:41:29 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_exit_code;

static void minishell(t_session *session, char **env)
{
	while (1)
	{
		/* handle_interactive_session_signal(); */
		sig_setup();
		session->input = readline("minishell>>");
		if (session->input == NULL)
			break;
		// if (session->input)
		// {
		// 	ft_printf("exit\n");
		// 	return (exit_shell(session, 0));
		// }
		/* handle_non_interactive_session_signal(); */
		add_history(session->input);
		lexer(session, env);
		parse_cmd();
		execute_cmd();
		free(session->input);
		/* if (lexer(session)) */
		/* { */
		/*     lstadd_back_token(&session->token, lstnew_token(NULL, END)); */
		/*     strip_space_token(&session->token); */
		/*     if (check_syntax(&session->token)) */
		/*         g_exit_code = 2; */
		/*     else */
		/*     { */
		/*         parser(session); */
		/*         g_exit_code = executor(session); */
		/*     } */
		/* } */
		/* free_session(session, false); */
	}
	printf("exit\n");
	exit_shell();
}

int main(int ac, char **av, char **env)
{
	t_session session;
	(void)av;
	//TODO remove when implemented env
	(void)env;

	// TODO: why use memset?
	ft_memset(&session, 0, sizeof(t_session));
	// if (argc != 1)
	// 	return (ft_putendl_fd("No arguments allowed!", 2), 1);
	if (ac != 1)
	{
		write(STDERR_FILENO, "No arguments allowed!\n", 22);
		return (1);
	}
	// TODO: implement init_session
	/* if (!init_session(&session, env)) */
	/*     exit_shell(&session, EXIT_FAILURE)) */
	minishell(&session, env);
	exit_shell(&session, g_exit_code);
	return (0);
}
