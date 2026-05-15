/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:34:56 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:49:03 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*create_cmd_node(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->infile = STDIN_FILENO;
	new_cmd->outfile = STDOUT_FILENO;
	new_cmd->append = false;
	new_cmd->limiter = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

void	add_cmd(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*cmds)
	{
		*cmds = new_cmd;
		return ;
	}
	current = *cmds;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

static int	count_args(t_token *tok)
{
	int	i;

	i = 0;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == CMD)
			i++;
		else if (tok->type != CMD)
			tok = tok->next;
		if (tok)
			tok = tok->next;
	}
	return (i);
}

static void	fill_cmd_args(t_cmd *cmd, t_token **tok)
{
	int	i;

	cmd->args = malloc(sizeof(char *) * (count_args(*tok) + 1));
	if (!cmd->args)
		return ;
	i = 0;
	while (*tok && (*tok)->type != PIPE)
	{
		if ((*tok)->type == CMD)
		{
			cmd->args[i++] = ft_strdup((*tok)->value);
			*tok = (*tok)->next;
		}
		else
		{
			handle_redirection(cmd, tok);
			if (*tok)
				*tok = (*tok)->next;
		}
	}
	cmd->args[i] = NULL;
}

t_cmd	*build_cmd_table(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*cur_cmd;
	t_token	*cur_tok;

	head = NULL;
	cur_tok = tokens;
	while (cur_tok)
	{
		cur_cmd = create_cmd_node();
		fill_cmd_args(cur_cmd, &cur_tok);
		add_cmd(&head, cur_cmd);
		if (cur_tok && cur_tok->type == PIPE)
			cur_tok = cur_tok->next;
	}
	return (head);
}
