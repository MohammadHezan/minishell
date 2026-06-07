/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:50:22 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/15 22:50:23 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	parse_input(t_shell *shell, char *input)
{
	if (!input || !*input)
		return (false);
	shell->tokens = tokenize_input(input);
	if (!shell->tokens)
		return (false);
	if (!check_syntax(shell->tokens))
	{
		free_cycle(shell);
		shell->exit_status = 258;
		return (false);
	}
	expand_tokens(shell);
	shell->current_cmd = build_cmd_table(shell, shell->tokens);
	return (true);
}
