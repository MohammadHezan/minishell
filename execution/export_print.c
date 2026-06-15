/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 13:26:00 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/15 13:26:00 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sort_env_array(t_env **array, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = -1;
	while (++i < count - 1)
	{
		j = i;
		while (++j < count)
		{
			if (ft_strcmp(array[i]->key, array[j]->key) > 0)
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}
}

static void	print_and_free(t_env **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
	{
		if (arr[i]->value)
			printf("declare -x %s=\"%s\"\n", arr[i]->key, arr[i]->value);
		else
			printf("declare -x %s\n", arr[i]->key);
	}
	free(arr);
}

void	print_sorted_env(t_shell *shell)
{
	t_env	**arr;
	t_env	*curr;
	int		i;

	arr = malloc(sizeof(t_env *) * (count_env(shell->env) + 1));
	if (!arr)
		return ;
	curr = shell->env;
	i = 0;
	while (curr)
	{
		arr[i++] = curr;
		curr = curr->next;
	}
	arr[i] = NULL;
	sort_env_array(arr, i);
	print_and_free(arr);
}
