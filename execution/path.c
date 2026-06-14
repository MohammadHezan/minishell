/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 10:26:14 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/07 06:49:59 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	has_path(char *cmd)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	*search_func(char **arr, char *cmd)
{
	int		j;
	char	*tmp;
	char	*search_path;

	j = 0;
	search_path = NULL;
	while (arr[j])
	{
		tmp = ft_strjoin(arr[j], "/");
		search_path = ft_strjoin(tmp, cmd);
		if (access(search_path, F_OK | X_OK) != -1)
		{
			free(tmp);
			return (search_path);
		}
		j++;
		free(tmp);
		free(search_path);
	}
	return (NULL);
}

char	*check_path(t_shell *shell, char *cmd)
{
	char	*paths;
	char	**arr;
	char	*search_path;

	paths = get_env_value(shell->env, "PATH");
	if (!paths)
		return (NULL);
	arr = ft_split(paths, ':');
	if (!arr)
		return (NULL);
	search_path = search_func(arr, cmd);
	free_arr(arr);
	return (search_path);
}

char	*cmd_path(t_shell *shell, char *cmd)
{
	char	*path;

	if (has_path(cmd))
		path = cmd;
	else
		path = check_path(shell, cmd);
	return (path);
}
