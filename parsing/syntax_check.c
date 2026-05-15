/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:13:54 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:19:11 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_syntax(t_token *tokens)
{
	t_token	*cur;

	if (!tokens)
		return (true);
	if (tokens->type == PIPE)
		return (false);
	cur = tokens;
	while (cur)
	{
		if (cur->type != CMD && cur->type != PIPE)
		{
			if (!cur->next || cur->next->type == PIPE)
				return (false);
			if (cur->next->type != CMD && cur->next->type != PIPE)
				return (false);
		}
		if (cur->type == PIPE && (!cur->next || cur->next->type == PIPE))
			return (false);
		cur = cur->next;
	}
	return (true);
}
