/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:52:07 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/15 22:52:08 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_val(t_shell *shell, char *key)
{
	t_env	*env;
	char	*exit_stat;

	if (ft_strncmp(key, "?", 2) == 0)
	{
		exit_stat = ft_itoa(shell->exit_status);
		return (exit_stat);
	}
	env = shell->env;
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

int	get_var_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*res;

	if (!s1)
		s1 = ft_strdup("");
	if (!s2)
		s2 = ft_strdup("");
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}
