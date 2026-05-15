/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:33:42 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:33:57 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_file(char *file, int mode)
{
	int	fd;

	if (mode == 0)
		fd = open(file, O_RDONLY);
	else if (mode == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(file);
		return (-1);
	}
	return (fd);
}

void	handle_redirection(t_cmd *cmd, t_token **tok)
{
	t_token_type	type;
	char			*file;

	type = (*tok)->type;
	*tok = (*tok)->next;
	if (!*tok)
		return ;
	file = (*tok)->value;
	if (type == REDIRECT_IN)
	{
		if (cmd->infile != STDIN_FILENO)
			close(cmd->infile);
		cmd->infile = open_file(file, 0);
	}
	else if (type == REDIRECT_OUT || type == APPEND)
	{
		if (cmd->outfile != STDOUT_FILENO)
			close(cmd->outfile);
		cmd->outfile = open_file(file, (type == APPEND) + 1);
	}
	else if (type == HEREDOC)
		cmd->limiter = ft_strdup(file);
}
