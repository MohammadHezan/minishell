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

/* Returns 1 if str contains any quotes */
static int	has_quotes(char *str)
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

/* Split a string by whitespace into new CMD tokens inserted after *tok_ptr.
   The original token is replaced by the first word; if empty, removed. */
static void	word_split_token(t_token **tok_ptr)
{
	t_token	*tok;
	t_token	*new_tok;
	t_token	*last;
	char	**words;
	int		i;

	tok = *tok_ptr;
	words = ft_split(tok->value, ' ');
	if (!words)
		return ;
	if (!words[0])
	{
		free(tok->value);
		tok->value = ft_strdup("");
		free_arr(words);
		return ;
	}
	free(tok->value);
	tok->value = ft_strdup(words[0]);
	last = tok;
	i = 1;
	while (words[i])
	{
		new_tok = create_tok(words[i], CMD, 0);
		if (new_tok)
		{
			new_tok->next = last->next;
			last->next = new_tok;
			last = new_tok;
		}
		i++;
	}
	free_arr(words);
}

void	expand_tokens(t_shell *shell)
{
	t_token	*tok;
	char	*new_val;
	int		was_quoted;

	tok = shell->tokens;
	while (tok)
	{
		if (tok->type == CMD)
		{
			was_quoted = has_quotes(tok->value);
			new_val = process_str(tok->value, shell);
			free(tok->value);
			tok->value = new_val;
			/* Word-split only if the original had no quotes */
			if (!was_quoted && ft_strchr(tok->value, ' '))
				word_split_token(&tok);
		}
		tok = tok->next;
	}
}
