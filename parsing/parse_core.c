/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:13:50 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:13:51 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	parse_input(t_shell *shell, char *input)
{
	shell->tokens = tokenize_input(input);
	if (!shell->tokens)
		return (false);
	if (!check_syntax(shell->tokens))
	{
		printf("minishell: syntax error\n");
		shell->exit_status = 258;
		return (false);
	}
	expand_tokens(shell);
	shell->current_cmd = build_cmd_table(shell->tokens);
	if (!shell->current_cmd)
		return (false);
	return (true);
}
