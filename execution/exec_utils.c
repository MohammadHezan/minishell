/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 11:49:50 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 11:49:50 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_directory(char *cmd, char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd);
		exit(126);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
}

char	*get_valid_cmd_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		if (cmd && (cmd->infile == -1 || cmd->outfile == -1))
			exit(1);
		exit(0);
	}
	path = cmd_path(shell, cmd->args[0]);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	return (path);
}

void	handle_parent_status(t_shell *shell, int status)
{
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

void	run_fork_child(t_shell *shell)
{
	exec_signals();
	setup_child_fds(shell->current_cmd, NULL, -1);
	child_process(shell, shell->current_cmd);
}

void	restore_fds(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}
