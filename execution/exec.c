/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 22:09:29 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/05/11 13:19:35 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

void	ft_env(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i])
	{
		printf("%s\n", env_array[i]);
		i++;
	}
}
// add to t_cmd a shell ponter

void	ft_unset(t_shell *shell)
{
	int		i;
	t_env	*tmp;

	i = 0;
	while (shell->env[i].key != NULL)
	{
		if (ft_strncmp(shell->env[i].key, shell->current_cmd->args[1],
				ft_strlen(shell->current_cmd->args[1])))
		{
			tmp = &shell->env[i];
			if (shell->env[i + 1].key != NULL)
				shell->env[i - 1].next = &shell->env[i + 1];
			else
				shell->env[i - 1].next = NULL;
			free(tmp);
		}
	}
}

void	ft_export(t_shell *shell)
{
}

void	ft_exit(void)
{
	exit(0);
}

void	built_in(t_shell *shell)
{
	if (ft_strncmp(shell->current_cmd[0].args[0], "echo", 5) == 0)
		ft_echo(shell->current_cmd);
	else if (ft_strncmp(shell->current_cmd[0].args[0], "cd", 3) == 0)
		ft_cd(shell->current_cmd);
	else if (ft_strncmp(shell->current_cmd[0].args[0], "pwd", 4) == 0)
		ft_pwd(shell->current_cmd);
	else if (ft_strncmp(shell->current_cmd[0].args[0], "export", 7) == 0)
		ft_export(shell);
	else if (ft_strncmp(shell->current_cmd[0].args[0], "unset", 6) == 0)
		ft_unset(shell);
	else if (ft_strncmp(shell->current_cmd[0].args[0], "env", 4) == 0)
		ft_env(shell->env_array);
	else if (ft_strncmp(shell->current_cmd[0].args[0], "exit", 5) == 0)
		ft_exit();
}
