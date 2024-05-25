/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:24:31 by xiwang            #+#    #+#             */
/*   Updated: 2024/05/25 20:48:56 by xiruwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand_arg(t_token *temp, t_cmd *cmd, int i)
{
	t_builtin	builtin;
	char		*new;

	new = NULL;
	new = expand_complex(temp->value, cmd->data);// added
	if (new && if_all_space(new) == 0)//"a   " trim
	{
		cmd->s[i] = ft_strtrim(new, " ");
		free(new);
	}
	else//if "   ", dont trim
		cmd->s[i] = new;
	if (i == 0 && ft_strncmp(cmd->s[i], "", 1) == 0)//eg. $hi//not sure
		cmd->empty_var = 1;//not sure
	else if (i == 0)
	{
		builtin = ft_builtin(cmd->s[i]);
		cmd->is_builtin = builtin;
	}
}

int	check_syntax(t_token *next, t_data *data)
{
	if (!next || !next->value)
	{
		data->exit_code = 2;
		printf("minishell: syntax error near "
			"unexpected token `newline\'\n");
		return (EXIT_FAILURE);
	}
	if (next->type != STR)
	{
		data->exit_code = 2;
		printf("minishell: syntax error near "
			"unexpected token `%s\'\n", next->value);
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
