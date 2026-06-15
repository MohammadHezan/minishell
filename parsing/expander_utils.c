/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:52:07 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 12:52:39 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_val(t_shell *shell, char *key)
{
	t_env	*env;

	if (ft_strncmp(key, "?", 2) == 0)
		return (ft_itoa(shell->exit_status));
	if (ft_strncmp(key, "$", 2) == 0)
		return (ft_itoa(getpid()));
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

	if (str[0] == '?' || str[0] == '$')
		return (1);
	if (ft_isdigit(str[0]))
		return (1);
	i = 0;
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

int	has_quotes(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

char	*append_char(char *res, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (join_and_free(res, ft_strdup(tmp)));
}
