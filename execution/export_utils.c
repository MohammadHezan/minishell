/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 11:45:10 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 11:47:02 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
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

static void	update_or_add_env(t_shell *shell, t_env *node, char *key, char *val)
{
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(val);
		free(key);
	}
	else if (!shell->env)
		shell->env = create_env_node(key, ft_strdup(val));
	else
		add_env_back(shell->env, create_env_node(key, ft_strdup(val)));
}

static void	export_no_eq(t_shell *shell, char *arg)
{
	t_env	*node;

	if (get_env_by_key(shell->env, arg))
		return ;
	node = malloc(sizeof(t_env));
	if (!node)
		return ;
	node->key = ft_strdup(arg);
	node->value = NULL;
	node->next = NULL;
	if (!shell->env)
		shell->env = node;
	else
		add_env_back(shell->env, node);
	shell->env_edited = true;
}

void	export_one(t_shell *shell, char *arg)
{
	char	*eq;
	char	*key;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		export_no_eq(shell, arg);
		return ;
	}
	key = ft_substr(arg, 0, eq - arg);
	update_or_add_env(shell, get_env_by_key(shell->env, key), key, eq + 1);
	shell->env_edited = true;
}
