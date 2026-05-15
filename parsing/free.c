/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:13:34 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:19:40 by mohammad-he      ###   ########.fr       */
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

static void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;
		i = 0;
		if (cmd->args)
		{
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		free(cmd);
		cmd = tmp;
	}
}

void	free_cycle(t_shell *shell)
{
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_cmds(shell->current_cmd);
	shell->current_cmd = NULL;
}

void	free_shell(t_shell *shell)
{
	t_env	*tmp;

	free_cycle(shell);
	while (shell->env)
	{
		tmp = shell->env->next;
		free(shell->env->key);
		free(shell->env->value);
		free(shell->env);
		shell->env = tmp;
	}
}
