/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:52:13 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/07 06:51:02 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_var(char *str, int *i, t_shell *shell)
{
	int		len;
	char	*key;
	char	*val;

	(*i)++;
	len = get_var_len(str + *i);
	if (len == 0)
		return (ft_strdup("$"));
	key = ft_substr(str, *i, len);
	val = get_env_val(shell, key);
	free(key);
	*i += len - 1;
	return (val);
}

static char	*process_str(char *str, t_shell *shell)
{
	char	*res;
	char	tmp[2];
	int		i;
	int		state;

	res = ft_strdup("");
	i = -1;
	state = 0;
	tmp[1] = '\0';
	while (str[++i])
	{
		if (str[i] == '\'' && state != 2)
			state = 1 - state;
		else if (str[i] == '\"' && state != 1)
			state = 2 - state;
		else if (str[i] == '$' && state != 1)
			res = join_and_free(res, extract_var(str, &i, shell));
		else
		{
			tmp[0] = str[i];
			res = join_and_free(res, ft_strdup(tmp));
		}
	}
	return (res);
}

void	expand_tokens(t_shell *shell)
{
	t_token	*tok;
	char	*new_val;

	tok = shell->tokens;
	while (tok)
	{
		if (tok->type == CMD)
		{
			new_val = process_str(tok->value, shell);
			free(tok->value);
			tok->value = new_val;
		}
		tok = tok->next;
	}
}
