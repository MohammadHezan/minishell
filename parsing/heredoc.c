/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:10:34 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/19 10:11:28 by mohammad-he      ###   ########.fr       */
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
			write(2, "minishell: warning: delimited by EOF\n", 38);
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

int	handle_heredoc(char *limiter)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	read_heredoc_lines(fd[1], limiter);
	close(fd[1]);
	return (fd[0]);
}
