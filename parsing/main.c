/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:12:42 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:19:04 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	shell->history = NULL;
	shell->pids = NULL;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	shell->env_array = NULL;
	shell->env = init_env(envp);
	shell->current_cmd = NULL;
	shell->tokens = NULL;
	shell->exit_status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (1)
	{
		write(1, "minishell$ ", 12);
		input = get_next_line(0);
		if (!input)
			break ;
		if (input[0] && input[0] != '\n')
		{
			if (parse_input(&shell, input))
				execute_commands(&shell);
			free_cycle(&shell);
		}
		free(input);
	}
	free_shell(&shell);
	return (shell.exit_status);
}
