/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 13:29:45 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 11:42:06 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->env_edited)
	{
		free(shell->env_array);
		shell->env_array = rebuild_env(shell);
	}
	while (shell->env_array[i])
	{
		if (ft_strchr(shell->env_array[i], '='))
			printf("%s\n", shell->env_array[i]);
		i++;
	}
	shell->exit_status = 0;
}

void	ft_cd(t_cmd *cmd)
{
	char	*path;
	char	*oldpwd;
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)))
		oldpwd = ft_strdup(cwd);
	else
		oldpwd = ft_strdup("");
	cmd->shell->exit_status = 0;
	path = get_target_path(cmd, oldpwd);
	if (!path && cmd->shell->exit_status == 1)
		return ;
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		free(oldpwd);
		cmd->shell->exit_status = 1;
		return ;
	}
	change_env_value(cmd->shell->env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		change_env_value(cmd->shell->env, "PWD", ft_strdup(cwd));
}

void	ft_pwd(t_shell *shell)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		shell->exit_status = 0;
	}
	else
		perror("minishell: pwd");
}
