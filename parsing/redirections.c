/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:53:02 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 12:42:03 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_redir_err(char *file)
{
	write(2, "minishell: ", 11);
	perror(file);
}

static void	open_infile(t_cmd *cmd, char *file)
{
	if (cmd->infile != STDIN_FILENO)
		close(cmd->infile);
	cmd->infile = open(file, O_RDONLY);
	if (cmd->infile == -1)
	{
		print_redir_err(file);
		cmd->shell->exit_status = 1;
	}
}

static void	open_outfile(t_cmd *cmd, char *file, t_token_type type)
{
	if (cmd->outfile != STDOUT_FILENO)
		close(cmd->outfile);
	if (type == REDIRECT_OUT || type == TRUNCATE)
	{
		cmd->outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		cmd->append = false;
	}
	else if (type == APPEND)
	{
		cmd->outfile = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		cmd->append = true;
	}
	if (cmd->outfile == -1)
	{
		print_redir_err(file);
		cmd->shell->exit_status = 1;
	}
}

static void	open_file(t_cmd *cmd, char *file, t_token_type type)
{
	if (cmd->infile == -1 || cmd->outfile == -1)
		return ;
	if (type == REDIRECT_IN)
		open_infile(cmd, file);
	else if (type == REDIRECT_OUT || type == TRUNCATE || type == APPEND)
		open_outfile(cmd, file, type);
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
	if (type == HEREDOC)
	{
		if (cmd->infile != STDIN_FILENO && cmd->infile != -1)
			close(cmd->infile);
		cmd->infile = handle_heredoc(cmd->shell, file);
	}
	else
		open_file(cmd, file, type);
	*tok = (*tok)->next;
}
