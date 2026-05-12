/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:13:38 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:19:44 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_tok(char *value, t_token_type type, int quote)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = value;
	new_node->type = type;
	new_node->quote_type = quote;
	new_node->next = NULL;
	return (new_node);
}

int	add_token(t_token **tokens, t_token *new_node)
{
	t_token	*current;

	if (!new_node)
		return (0);
	if (!*tokens)
		*tokens = new_node;
	else
	{
		current = *tokens;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (1);
}
