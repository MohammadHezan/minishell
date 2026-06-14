/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:50:22 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/07 13:00:25 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	parse_input(t_shell *shell, char *input)
{
	if (!input || !*input)
		return (false);
	if (input[0] == '\n')
	{
		free(input);
		return (false);
	}
	shell->tokens = tokenize_input(input);
	if (!shell->tokens)
		return (false);
	if (!check_syntax(shell->tokens))
	{
		free_cycle(shell);
		shell->exit_status = 2;
		return (false);
	}
	expand_tokens(shell);
	shell->current_cmd = build_cmd_table(shell, shell->tokens);
	return (true);
}
