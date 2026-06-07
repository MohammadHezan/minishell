/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:09:00 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/07 14:23:45 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_shell *shell, t_cmd *cmd)
{
	t_env	*curr;
	t_env	*prev;

	if (!cmd->args[1])
		return ;
	curr = shell->env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, cmd->args[1]) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				shell->env = curr->next;
			free_env_node(curr);
			shell->env_edited = true;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	shell->exit_status = 0;
}

static t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

static void	add_env_back(t_env *env, t_env *new_node)
{
	t_env	*curr;

	curr = env;
	while (curr->next)
	{
		curr = curr->next;
	}
	curr->next = new_node;
}

void	ft_export(t_shell *shell, t_cmd *cmd)
{
	t_env	*node;
	char	*eq;
	char	*key;

	if (!cmd->args[1])
	{
		ft_env(shell);
		shell->exit_status = 0;
		return ;
	}
	eq = ft_strchr(cmd->args[1], '=');
	if (!eq)
		return ;
	key = ft_substr(cmd->args[1], 0, eq - cmd->args[1]);
	node = get_env_by_key(shell->env, key);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(eq + 1);
		free(key);
	}
	else
	{
		if (!shell->env)
			shell->env = create_env_node(key, ft_strdup(eq + 1));
		else
			add_env_back(shell->env, create_env_node(key, ft_strdup(eq + 1)));
	}
	shell->exit_status = 0;
	shell->env_edited = true;
}
