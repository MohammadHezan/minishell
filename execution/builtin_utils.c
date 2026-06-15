/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 11:38:00 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 11:46:56 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_home(t_cmd *cmd, char *oldpwd)
{
	char	*path;

	path = get_env_value(cmd->shell->env, "HOME");
	if (!path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		free(oldpwd);
		cmd->shell->exit_status = 1;
	}
	return (path);
}

static char	*handle_oldpwd(t_cmd *cmd, char *oldpwd)
{
	char	*path;

	path = get_env_value(cmd->shell->env, "OLDPWD");
	if (!path)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		free(oldpwd);
		cmd->shell->exit_status = 1;
	}
	else
		ft_putendl_fd(path, 1);
	return (path);
}

char	*get_target_path(t_cmd *cmd, char *oldpwd)
{
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		free(oldpwd);
		cmd->shell->exit_status = 2;
		return (NULL);
	}
	if (!cmd->args[1])
		return (handle_home(cmd, oldpwd));
	if (ft_strcmp(cmd->args[1], "-") == 0)
		return (handle_oldpwd(cmd, oldpwd));
	return (cmd->args[1]);
}

int	is_exit_numeric(char *s)
{
	int	i;
	int	sign;

	i = skip_spaces_and_sign(s, &sign);
	if (!s[i] || !ft_isdigit(s[i]))
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	i = skip_spaces_and_sign(s, &sign);
	return (check_overflow(s + i, sign));
}
