/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:51:45 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 12:52:36 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_tokens(t_token *tok)
{
	t_token	*tmp;

	while (tok)
	{
		tmp = tok->next;
		free(tok->value);
		free(tok);
		tok = tmp;
	}
}

static void	free_single_cmd(t_cmd *cmd)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	if (cmd->limiter)
		free(cmd->limiter);
	if (cmd->infile != STDIN_FILENO && cmd->infile != -1)
		close(cmd->infile);
	if (cmd->outfile != STDOUT_FILENO && cmd->outfile != -1)
		close(cmd->outfile);
	free(cmd);
}

static void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_single_cmd(cmd);
		cmd = tmp;
	}
}

void	free_cycle(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->current_cmd)
	{
		free_cmds(shell->current_cmd);
		shell->current_cmd = NULL;
	}
}
