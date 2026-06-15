/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:02:04 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 12:41:55 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	wait_for_children(t_shell *shell, int cmd_count, int failed)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < cmd_count)
	{
		waitpid(shell->pids[i], &status, 0);
		if (i == cmd_count - 1 && !failed)
			handle_parent_status(shell, status);
		i++;
	}
	if (failed)
		shell->exit_status = 1;
	free(shell->pids);
	shell->pids = NULL;
}

static void	handle_pipe_parent(t_cmd *cmd, int *pipe_fd, int *prev_fd)
{
	if (*prev_fd != -1)
	{
		close(*prev_fd);
		*prev_fd = -1;
	}
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
	t_cmd	*cmd;
	int		pipe_fd[2];
	int		prev_fd;
	int		i;

	if (!allocate_pids(shell))
		return ;
	init_pipe_vars(&cmd, shell, &prev_fd, &i);
	while (cmd)
	{
		shell->pids[i] = fork_pipe_child(shell, cmd, pipe_fd, prev_fd);
		if (shell->pids[i] == -1)
			break ;
		handle_pipe_parent(cmd, pipe_fd, &prev_fd);
		cmd = cmd->next;
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_for_children(shell, i, cmd != NULL);
	init_signals();
}
