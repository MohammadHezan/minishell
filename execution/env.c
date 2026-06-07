/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa </var/spool/mail/zaalrafa>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 00:02:36 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/05/11 00:02:48 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_env_str(char *key, char *value)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

char	**rebuild_env(t_shell *shell)
{
	t_env	*curr;
	char	**array;
	int		i;

	curr = shell->env;
	array = malloc(sizeof(char *) * (count_env(curr) + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (curr)
	{
		array[i] = get_env_str(curr->key, curr->value);
		if (!array[i])
		{
			free_arr(array);
			return (NULL);
		}
		curr = curr->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}

bool	change_env_value(t_env *env, char *key, char *value)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = value;
			return (true);
		}
		curr = curr->next;
	}
	return (false);
}

t_env	*get_env_by_key(t_env *env, char *key)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}
