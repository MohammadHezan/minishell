/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:52:13 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/06/14 12:39:09 by zaalrafa         ###   ########.fr       */
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
	{
		(*i)--;
		return (ft_strdup("$"));
	}
	key = ft_substr(str, *i, len);
	val = get_env_val(shell, key);
	free(key);
	*i += len - 1;
	return (val);
}

static char	*process_str(char *str, t_shell *shell)
{
	char	*res;
	int		i;
	int		state;

	res = ft_strdup("");
	i = -1;
	state = 0;
	while (str[++i])
	{
		if (str[i] == '\'' && state != 2)
			state = 1 - state;
		else if (str[i] == '\"' && state != 1)
			state = 2 - state;
		else if (str[i] == '$' && state != 1)
			res = join_and_free(res, extract_var(str, &i, shell));
		else
			res = append_char(res, str[i]);
	}
	return (res);
}

char	*expand_heredoc_line(char *str, t_shell *shell)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
			res = join_and_free(res, extract_var(str, &i, shell));
		else
			res = append_char(res, str[i]);
	}
	free(str);
	return (res);
}

static void	expand_cmd_token(t_token *tok, t_shell *shell)
{
	int		was_quoted;
	char	*new_val;

	was_quoted = has_quotes(tok->value);
	new_val = process_str(tok->value, shell);
	free(tok->value);
	tok->value = new_val;
	if (!was_quoted && ft_strchr(tok->value, ' '))
		word_split_token(&tok);
}

void	expand_tokens(t_shell *shell)
{
	t_token	*tok;
	char	*new_val;

	tok = shell->tokens;
	while (tok)
	{
		if (tok->type == HEREDOC && tok->next && tok->next->type == CMD)
		{
			tok->next->quote_type = has_quotes(tok->next->value);
			new_val = strip_quotes(tok->next->value);
			free(tok->next->value);
			tok->next->value = new_val;
			tok = tok->next;
		}
		else if (tok->type == CMD)
			expand_cmd_token(tok, shell);
		tok = tok->next;
	}
}
