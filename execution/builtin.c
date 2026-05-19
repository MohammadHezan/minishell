/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 13:29:45 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/05/19 10:18:06 by mohammad-he      ###   ########.fr       */
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
		printf("%s\n", shell->env_array[i]);
		i++;
	}
}

void	ft_cd(t_cmd *cmd)
{
	char	*path;
	char	cwd[4096];

	path = cmd->args[1];
	if (!path)
		path = get_env_value(cmd->shell->env, "HOME");
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)))
		change_env_value(cmd->shell->env, "PWD", cwd);
}

// creat a cleanup_shell function that closes and frees everything
void	ft_exit(t_shell *shell)
{
	free_shell(shell);
	exit(EXIT_SUCCESS);
}

void	ft_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("minishell: pwd");
}
