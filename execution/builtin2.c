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
	int		i;

	if (!cmd->args[1])
		return ;
	i = 1;
	while (cmd->args[i])
	{
		curr = shell->env;
		prev = NULL;
		while (curr)
		{
			if (ft_strcmp(curr->key, cmd->args[i]) == 0)
			{
				if (prev)
					prev->next = curr->next;
				else
					shell->env = curr->next;
				free_env_node(curr);
				shell->env_edited = true;
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
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

static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	export_one(t_shell *shell, char *arg)
{
	t_env	*node;
	char	*eq;
	char	*key;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		/* export VAR without value: mark as exported (add to env if not present,
		   but without a value — matches bash where it appears in `export -p`
		   but not in `env`). We skip if already present. */
		if (!get_env_by_key(shell->env, arg))
		{
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
		return ;
	}
	key = ft_substr(arg, 0, eq - arg);
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
	shell->env_edited = true;
}

void	ft_export(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*eq;
	char	*key;
	int		len;

	if (!cmd->args[1])
	{
		ft_env(shell);
		shell->exit_status = 0;
		return ;
	}
	shell->exit_status = 0;
	i = 1;
	while (cmd->args[i])
	{
		eq = ft_strchr(cmd->args[i], '=');
		if (eq)
			len = eq - cmd->args[i];
		else
			len = ft_strlen(cmd->args[i]);
		key = ft_substr(cmd->args[i], 0, len);
		if (!is_valid_identifier(key))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			shell->exit_status = 1;
			free(key);
			i++;
			continue ;
		}
		free(key);
		export_one(shell, cmd->args[i]);
		i++;
	}
}
