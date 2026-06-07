/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 22:09:29 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/07 17:22:23 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_shell *shell, t_cmd *cmd)
{
	char		*cmd_pt;
	struct stat	path_stat;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		if (cmd && (cmd->infile == -1 || cmd->outfile == -1))
			exit(1);
		exit(0);
	}
	cmd_pt = cmd_path(shell, cmd->args[0]);
	if (!cmd_pt)
	{
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (stat(cmd_pt, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
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

	if (shell->current_cmd->infile == -1 || shell->current_cmd->outfile == -1)
	{
		if (shell->exit_status != 130)
			shell->exit_status = 1;
		return ;
	}
	ignore_signals();
	pid = fork();
	if (pid == -1)
		error_message("fork", 1);
	if (pid == 0)
	{
		exec_signals();
		setup_child_fds(shell->current_cmd, NULL, -1);
		child_process(shell, shell->current_cmd);
	}
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

static void	exec_single_builtin(t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	if (shell->current_cmd->infile == -1 || shell->current_cmd->outfile == -1)
	{
		if (shell->exit_status != 130)
			shell->exit_status = 1;
		return ;
	}
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	setup_child_fds(shell->current_cmd, NULL, -1);
	if (shell->current_cmd->args && shell->current_cmd->args[0]
		&& ft_strcmp(shell->current_cmd->args[0], "exit") == 0)
	{
		close(saved_stdin);
		close(saved_stdout);
		built_in(shell->current_cmd);
		return ;
	}
	built_in(shell->current_cmd);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	exec(t_shell *shell)
{
	if (!shell->current_cmd)
		return ;
	if (!shell->current_cmd->args || !shell->current_cmd->args[0])
	{
		if (shell->current_cmd->infile == -1 || shell->current_cmd->outfile ==
			-1)
		{
			if (shell->exit_status != 130)
				shell->exit_status = 1;
		}
		else
			shell->exit_status = 0;
		return ;
	}
	if (shell->current_cmd->next || shell->current_cmd->pipe)
		exec_pipe(shell);
	else
	{
		if (check_built_in(shell->current_cmd))
			exec_single_builtin(shell);
		else
			exec_external(shell);
	}
}
