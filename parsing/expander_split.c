/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:13:00 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/15 11:13:00 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	insert_split_tokens(t_token *last, char **words)
{
	t_token	*new_tok;
	int		i;

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
}

void	word_split_token(t_token **tok_ptr)
{
	t_token	*tok;
	char	**words;

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
	insert_split_tokens(tok, words);
	free_arr(words);
}

char	*strip_quotes(char *str)
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
		else
			res = append_char(res, str[i]);
	}
	return (res);
}
