/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:13:19 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:19:33 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	append_env(t_env **head, t_env *new_node)
{
	t_env	*cur;

	if (!*head)
		*head = new_node;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new_node;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	int		i;
	int		j;

	head = NULL;
	i = -1;
	while (envp && envp[++i])
	{
		new_node = malloc(sizeof(t_env));
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		new_node->key = ft_substr(envp[i], 0, j);
		new_node->value = ft_strdup(envp[i] + j + 1);
		new_node->next = NULL;
		append_env(&head, new_node);
	}
	return (head);
}
