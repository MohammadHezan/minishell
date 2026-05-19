/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_mock.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:52:25 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/15 22:54:42 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_cmd_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
	}
}

void	execute_commands(t_shell *shell)
{
	t_cmd	*cmd;
	int		cmd_num;

	cmd = shell->current_cmd;
	cmd_num = 1;
	while (cmd)
	{
		printf("--- CMD Node %d ---\n", cmd_num++);
		print_cmd_args(cmd);
		printf("  Infile fd: %d\n", cmd->infile);
		printf("  Outfile fd: %d\n", cmd->outfile);
		printf("  Append mode: %d\n", cmd->append);
		if (cmd->limiter)
			printf("  Heredoc Limiter: %s\n", cmd->limiter);
		cmd = cmd->next;
	}
}
