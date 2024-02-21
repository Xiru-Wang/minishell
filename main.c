/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 11:48:03 by jschroed          #+#    #+#             */
/*   Updated: 2024/02/21 16:39:47 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int g_exit_code;

// static void minishell(t_session *session)
// {
// 	//while (1)
// 	//{
// 		char *line;
// 		line = "hi ls | jshd  >   	";
// 		//session->line = readline("minishell>>");
// 		//if (session->line == NULL)
// 		//	break;
// 		printf("before split_line");
// 		session->tokens = split_line(line, session);
// 		int i = 0;
// 		printf("before loop");
// 		while (session->tokens[i])
// 		{
// 			printf("in the loop");
// 			printf("%s\n", session->tokens[i]);
// 			i++;
// 		}
// 		free_double_ptr(session->tokens);
// 		//lexer(session, env);
// 	//}
// 	printf("exit\n");
// 	//exit_shell();
// }


//int	main(int ac, char **av, char **env)
int main()
{
	//t_session	session;
	//(void)av;
	//(void)env;


	//ft_memset(&session, 0, sizeof(t_session));
	// if (ac != 1)
	// {
	// 	write(STDERR_FILENO, "No arguments allowed!\n", 22);
	// 	return (1);
	// }
	//minishell(&session);
		char *line;
		char **tokens;
		line = "  hello 42 ? |>< cat|  	";
		//session->line = readline("minishell>>");
		//if (session->line == NULL)
		//	break;
		printf("before split_line\n");
		tokens = split_line(line);
		int i = 0;
		while (tokens[i])
		{
			printf("token[%d]:%s\n", i, tokens[i]);
			i++;
		}
		free_double_ptr(tokens);
		//lexer(session, env);
	//}
	printf("exit\n");
	return (0);
}
