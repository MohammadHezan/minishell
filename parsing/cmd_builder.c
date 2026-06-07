/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:48:36 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/15 22:48:37 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*create_cmd_node(t_shell *shell)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->infile = STDIN_FILENO;
	node->outfile = STDOUT_FILENO;
	node->append = false;
	node->pipe = false;
	node->limiter = NULL;
	node->next = NULL;
	node->shell = shell;
	return (node);
}

void	add_cmd(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*temp;

	if (!new_cmd)
		return ;
	if (!*cmds)
	{
		*cmds = new_cmd;
		return ;
	}
	temp = *cmds;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
}

static int	count_args(t_token *tok)
{
	int	i;

	i = 0;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == CMD)
			i++;
		else if (tok->type != PIPE)
			tok = tok->next;
		if (tok)
			tok = tok->next;
	}
	return (i);
}

static void	fill_args(t_cmd *cmd, t_token **tok)
{
	int	i;
	int	count;

	count = count_args(*tok);
	cmd->args = malloc(sizeof(char *) * (count + 1));
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
			handle_redirection(cmd, tok);
	}
	cmd->args[i] = NULL;
}

t_cmd	*build_cmd_table(t_shell *shell, t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token	*tok;

	head = NULL;
	tok = tokens;
	while (tok)
	{
		curr = create_cmd_node(shell);
		fill_args(curr, &tok);
		add_cmd(&head, curr);
		if (tok && tok->type == PIPE)
			tok = tok->next;
	}
	return (head);
}
