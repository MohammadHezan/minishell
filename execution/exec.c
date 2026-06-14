/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 22:09:29 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 11:50:00 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_invalid_fds(t_shell *shell)
{
	if (shell->current_cmd->infile == -1 || shell->current_cmd->outfile == -1)
	{
		if (shell->exit_status != 130)
			shell->exit_status = 1;
		return (1);
	}
	return (0);
}

void	child_process(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_pt;

	cmd_pt = get_valid_cmd_path(shell, cmd);
	check_directory(cmd->args[0], cmd_pt);
	if (shell->env_edited)
		shell->env_array = rebuild_env(shell);
	execve(cmd_pt, cmd->args, shell->env_array);
	perror(cmd->args[0]);
	exit(126);
}

void	exec_external(t_shell *shell)
{
	int		status;
	pid_t	pid;

	if (check_invalid_fds(shell))
		return ;
	ignore_signals();
	fflush(stdout);
	pid = fork();
	if (pid == -1)
		error_message("fork", 1);
	if (pid == 0)
		run_fork_child(shell);
	waitpid(pid, &status, 0);
	init_signals();
	handle_parent_status(shell, status);
}

static void	exec_single_builtin(t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	if (check_invalid_fds(shell))
		return ;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	setup_child_fds(shell->current_cmd, NULL, -1);
	if (shell->current_cmd->args && shell->current_cmd->args[0]
		&& ft_strcmp(shell->current_cmd->args[0], "exit") == 0)
	{
		restore_fds(saved_stdin, saved_stdout);
		built_in(shell->current_cmd);
		return ;
	}
	built_in(shell->current_cmd);
	fflush(stdout);
	restore_fds(saved_stdin, saved_stdout);
}

void	exec(t_shell *shell)
{
	if (!shell->current_cmd)
		return ;
	if (!shell->current_cmd->args || !shell->current_cmd->args[0])
	{
		if (!check_invalid_fds(shell))
			shell->exit_status = 0;
		return ;
	}
	if (shell->current_cmd->next || shell->current_cmd->pipe)
		exec_pipe(shell);
	else if (check_built_in(shell->current_cmd))
		exec_single_builtin(shell);
	else
		exec_external(shell);
}
