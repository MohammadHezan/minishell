/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_mock.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:12:52 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:39:40 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_commands(t_shell *shell)
{
	t_cmd	*cmd;
	int		i;
	int		c;

	cmd = shell->current_cmd;
	c = 1;
	while (cmd)
	{
		printf("--- PARSED COMMAND TABLE NODE %d ---\n", c++);
		i = 0;
		if (cmd->args)
		{
			while (cmd->args[i])
			{
				printf("Arg[%d]: [%s]\n", i, cmd->args[i]);
				i++;
			}
		}
		if (cmd->infile != STDIN_FILENO)
			printf("infile: %d\n", cmd->infile);
		if (cmd->outfile != STDOUT_FILENO)
			printf("outfile: %d\n", cmd->outfile);
		if (cmd->append)
			printf("append: true\n");
		if (cmd->limiter)
			printf("limiter: [%s]\n", cmd->limiter);
		cmd = cmd->next;
	}
}
