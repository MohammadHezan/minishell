/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:48:57 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/15 22:48:58 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*create_env_node(char *str)
{
	t_env	*node;
	char	*eq;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	eq = ft_strchr(str, '=');
	if (eq)
	{
		node->key = ft_substr(str, 0, eq - str);
		node->value = ft_strdup(eq + 1);
	}
	else
	{
		node->key = ft_strdup(str);
		node->value = NULL;
	}
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*curr;
	int		i;

	head = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (!head)
		{
			head = create_env_node(envp[i]);
			curr = head;
		}
		else
		{
			curr->next = create_env_node(envp[i]);
			curr = curr->next;
		}
		i++;
	}
	return (head);
}
