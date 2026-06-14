/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:09:00 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 11:46:49 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	unset_one(t_shell *shell, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = shell->env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
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
}

void	ft_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	if (!cmd->args[1])
		return ;
	i = 1;
	while (cmd->args[i])
	{
		unset_one(shell, cmd->args[i]);
		i++;
	}
	shell->exit_status = 0;
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

static int	check_export_arg(t_shell *shell, char *arg)
{
	char	*eq;
	char	*key;
	int		len;

	eq = ft_strchr(arg, '=');
	if (eq)
		len = eq - arg;
	else
		len = ft_strlen(arg);
	key = ft_substr(arg, 0, len);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		shell->exit_status = 1;
		free(key);
		return (0);
	}
	free(key);
	return (1);
}

void	ft_export(t_shell *shell, t_cmd *cmd)
{
	int	i;

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
		if (check_export_arg(shell, cmd->args[i]))
			export_one(shell, cmd->args[i]);
		i++;
	}
}
