/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_checkers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 13:27:40 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/07 06:49:59 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	built_in(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		ft_cd(cmd);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		ft_export(cmd->shell, cmd);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		ft_unset(cmd->shell, cmd);
	else if (ft_strncmp(cmd[0].args[0], "env", 4) == 0)
		ft_env(cmd->shell);
	else if (ft_strncmp(cmd[0].args[0], "exit", 5) == 0)
		ft_exit(cmd);
}

int	check_built_in(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (1);
	return (0);
}
