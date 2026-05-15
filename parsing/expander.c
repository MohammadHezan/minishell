/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:35:17 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:35:42 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_env_val(t_shell *shell, char *key)
{
	t_env	*env;
	size_t	len;

	if (key[0] == '?')
		return (ft_itoa(shell->exit_status));
	env = shell->env;
	len = ft_strlen(key);
	while (env)
	{
		if (!ft_strncmp(env->key, key, len) && ft_strlen(env->key) == len)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

void	expand_tokens(t_shell *shell)
{
	t_token	*cur;
	char	*new_val;

	cur = shell->tokens;
	while (cur)
	{
		if (cur->type == HEREDOC)
		{
			if (cur->next)
				cur = cur->next->next;
			else
				cur = cur->next;
			continue ;
		}
		if (cur->type == CMD && cur->quote_type != 1 && cur->value[0] == '$')
		{
			new_val = get_env_val(shell, cur->value + 1);
			free(cur->value);
			cur->value = new_val;
		}
		cur = cur->next;
	}
}
