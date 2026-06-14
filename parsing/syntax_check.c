/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:49:13 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/15 22:49:14 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_redirection(t_token_type type)
{
	if (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == TRUNCATE || type == APPEND || type == HEREDOC)
		return (true);
	return (false);
}

static bool	check_pipe_syntax(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	if (curr && curr->type == PIPE)
		return (false);
	while (curr)
	{
		if (curr->type == PIPE)
		{
			if (!curr->next || curr->next->type == PIPE)
				return (false);
		}
		curr = curr->next;
	}
	return (true);
}

static bool	check_redir_syntax(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (is_redirection(curr->type))
		{
			if (!curr->next || is_redirection(curr->next->type)
				|| curr->next->type == PIPE)
				return (false);
		}
		curr = curr->next;
	}
	return (true);
}

bool	check_syntax(t_token *tokens)
{
	if (!tokens)
		return (false);
	if (!check_pipe_syntax(tokens))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (false);
	}
	if (!check_redir_syntax(tokens))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
		return (false);
	}
	return (true);
}
