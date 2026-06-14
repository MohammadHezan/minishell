/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:51:19 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 12:42:12 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_tok(char *value, t_token_type type, int quote)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(value);
	new_node->type = type;
	new_node->quote_type = quote;
	new_node->next = NULL;
	return (new_node);
}

int	add_token(t_token **tokens, t_token *new_node)
{
	t_token	*temp;

	if (!new_node)
		return (0);
	if (!*tokens)
	{
		*tokens = new_node;
		return (1);
	}
	temp = *tokens;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
	return (1);
}
