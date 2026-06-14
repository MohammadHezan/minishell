/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 12:11:03 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 12:40:00 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_child_fds(t_cmd *cmd, int *pipe_fd, int prev_fd)
{
	if (cmd->infile > 0)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	else if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (cmd->outfile > 1)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
	else if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (prev_fd != -1)
		close(prev_fd);
}

void	init_pipe_vars(t_cmd **cmd, t_shell *shell, int *prev_fd, int *i)
{
	*cmd = shell->current_cmd;
	*prev_fd = -1;
	*i = 0;
	ignore_signals();
	fflush(stdout);
}

static void	handle_pipe_child(t_shell *shell, t_cmd *cmd, int *pipe_fd,
		int prev_fd)
{
	exec_signals();
	setup_child_fds(cmd, pipe_fd, prev_fd);
	if (check_built_in(cmd))
	{
		built_in(cmd);
		fflush(stdout);
		_exit(cmd->shell->exit_status);
	}
	child_process(shell, cmd);
}

int	fork_pipe_child(t_shell *shell, t_cmd *cmd, int *pipe_fd, int prev_fd)
{
	pid_t	pid;

	if (cmd->next && pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		handle_pipe_child(shell, cmd, pipe_fd, prev_fd);
	return (pid);
}
