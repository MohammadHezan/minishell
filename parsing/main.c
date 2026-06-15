/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:50:33 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 16:52:44 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void		init_signals(void);

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env(envp);
	shell->env_array = NULL;
	if (shell->env_array)
	{
		free_arr(shell->env_array);
		shell->env_array = NULL;
	}
	shell->env_array = rebuild_env(shell);
	shell->history = NULL;
	shell->pids = NULL;
	shell->tokens = NULL;
	shell->current_cmd = NULL;
	shell->exit_status = 0;
	shell->env_edited = false;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
}

void	free_shell(t_shell *shell)
{
	t_env	*tmp;

	free_cycle(shell);
	while (shell->env)
	{
		tmp = shell->env->next;
		free(shell->env->key);
		if (shell->env->value)
			free(shell->env->value);
		free(shell->env);
		shell->env = tmp;
	}
	close(shell->stdin_backup);
	close(shell->stdout_backup);
	if (shell->env_array)
		free_arr(shell->env_array);
	if (shell->pids)
		free(shell->pids);
}

static int	process_input_loop(t_shell *shell)
{
	char	*input;

	init_signals();
	input = readline("minishell$ ");
	if (g_sig_status == SIGINT)
	{
		shell->exit_status = 130;
		g_sig_status = 0;
	}
	if (!input)
	{
		shell->exit_status = 0;
		return (0);
	}
	if (*input)
		add_history(input);
	if (parse_input(shell, input))
		exec(shell);
	free_cycle(shell);
	free(input);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (1)
	{
		if (!process_input_loop(&shell))
			break ;
	}
	free_shell(&shell);
	return (shell.exit_status);
}
