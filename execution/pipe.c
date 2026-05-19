/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:02:04 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/19 11:03:57 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_pipe_child(t_shell *shell, int *pipe_fd, int fd_in)
{
	dup2(fd_in, STDIN_FILENO);
	if (shell->current_cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	child_process(shell, shell->current_cmd);
	exit(1);
}

static void	handle_pipe_parent(t_shell *shell, int *pipe_fd, int *fd_in)
{
	wait(NULL);
	close(pipe_fd[1]);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	shell->current_cmd = shell->current_cmd->next;
}

void	exec_pipe(t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		fd_in;

	fd_in = STDIN_FILENO;
	while (shell->current_cmd)
	{
		if (pipe(pipe_fd) == -1)
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
			handle_pipe_child(shell, pipe_fd, fd_in);
		else
			handle_pipe_parent(shell, pipe_fd, &fd_in);
	}
}
