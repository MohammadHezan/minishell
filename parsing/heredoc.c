/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:10:34 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 12:52:33 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	read_heredoc_lines(int fd, char *limiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2,
				"minishell: warning: here-document delimited by end-of-file\n",
				60);
			break ;
		}
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

static void	run_heredoc_child(int write_fd, char *limiter)
{
	signal(SIGINT, SIG_DFL);
	read_heredoc_lines(write_fd, limiter);
	close(write_fd);
	exit(0);
}

static int	handle_parent_heredoc(t_shell *shell, int read_fd, int status)
{
	init_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		shell->exit_status = 130;
		close(read_fd);
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		shell->exit_status = 130;
		close(read_fd);
		return (-1);
	}
	return (read_fd);
}

static int	fork_heredoc(int *fd, pid_t *pid)
{
	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	*pid = fork();
	if (*pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	return (0);
}

int	handle_heredoc(t_shell *shell, char *limiter)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (fork_heredoc(fd, &pid) == -1)
		return (-1);
	if (pid == 0)
		run_heredoc_child(fd[1], limiter);
	close(fd[1]);
	waitpid(pid, &status, 0);
	return (handle_parent_heredoc(shell, fd[0], status));
}
