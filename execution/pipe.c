/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:02:04 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/07 17:20:03 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	wait_for_children(t_shell *shell, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(shell->pids[i], &status, 0);
		if (i == cmd_count - 1)
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
		i++;
	}
	free(shell->pids);
	shell->pids = NULL;
}

void	setup_child_fds(t_cmd *cmd, int *pipe_fd, int prev_fd)
{
	if (cmd->infile > 0)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
		cmd->infile = -1;
	}
	else if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (cmd->outfile > 1)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
		cmd->outfile = -1;
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

static void	handle_pipe_parent(t_cmd *cmd, int *pipe_fd, int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}

static int	allocate_pids(t_shell *shell)
{
	t_cmd	*cmd;
	int		count;

	count = 0;
	cmd = shell->current_cmd;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	shell->pids = malloc(sizeof(pid_t) * count);
	if (!shell->pids)
		return (0);
	return (1);
}

void	exec_pipe(t_shell *shell)
{
	t_cmd *cmd;
	int pipe_fd[2];
	int prev_fd;
	int i;

	if (!allocate_pids(shell))
		return ;
	cmd = shell->current_cmd;
	prev_fd = -1;
	i = 0;
	ignore_signals();
	fflush(stdout);
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return ;
		shell->pids[i] = fork();
		if (shell->pids[i] == -1)
			return ;
		if (shell->pids[i] == 0)
			handle_pipe_child(shell, cmd, pipe_fd, prev_fd);
		handle_pipe_parent(cmd, pipe_fd, &prev_fd);
		cmd = cmd->next;
		i++;
	}
	wait_for_children(shell, i);
	init_signals();
}
