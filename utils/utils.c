/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa </var/spool/mail/zaalrafa>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 13:52:32 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/05/10 19:26:58 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_message(char *str, int exit_code)
{
	printf("%s\n", str);
	exit(exit_code);
}

int	count_env(t_env *env)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
