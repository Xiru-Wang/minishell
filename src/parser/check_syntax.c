/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:31 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/27 18:13:57 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand_arg(t_token *temp, t_token *next, t_cmd *cmd, int *i)
{
	t_builtin	builtin;

	cmd->s[*i] = expand_complex(temp->value, cmd->data);
	if (ft_strncmp(cmd->s[*i], "", 1) == 0 && next && next->type == STR)
		free(cmd->s[*i]);
	else
	{
		if (*i == 0 && ft_strncmp(cmd->s[*i], "", 1) == 0)
			cmd->empty_var = 1;
		if (*i == 0)
		{
			builtin = ft_builtin(cmd->s[*i]);
			cmd->is_builtin = builtin;
		}
		(*i)++;
	}
}

int	check_syntax(t_token *next, t_data *data)
{
	if (!next || !next->value)
	{
		data->exit_code = 2;
		ft_putendl_fd("minishell: syntax error near unexpected token \
				`newline\'\n",
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (next->type != STR)
	{
		data->exit_code = 2;
		print_error("minishell: syntax error near unexpected token ", \
				next->value, "\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	pipe_syntax(t_token *token, t_data *data)
{
	if (token->type == PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		data->exit_code = 2;
		return (1);
	}
	return (0);
}

int	check_empty_token(t_token *token, t_data *data)
{
	if (ft_strncmp(token->value, "\"\"", 3) == 0)
	{
		ft_putendl_fd("minishell: : command not found", STDERR_FILENO);
		data->exit_code = 127;
		return (1);
	}
	return (0);
}
