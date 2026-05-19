/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:51:10 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/15 22:51:12 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	skip_spaces(char *in, int i)
{
	while (in[i] && (in[i] == ' ' || in[i] == '\t'))
		i++;
	return (i);
}

static int	extract_operator(char *in, int i, t_token **toks)
{
	if (in[i] == '>' && in[i + 1] == '>')
		return (add_token(toks, create_tok(">>", APPEND, 0)), 2);
	if (in[i] == '<' && in[i + 1] == '<')
		return (add_token(toks, create_tok("<<", HEREDOC, 0)), 2);
	if (in[i] == '>')
		return (add_token(toks, create_tok(">", TRUNCATE, 0)), 1);
	if (in[i] == '<')
		return (add_token(toks, create_tok("<", REDIRECT_IN, 0)), 1);
	if (in[i] == '|')
		return (add_token(toks, create_tok("|", PIPE, 0)), 1);
	return (0);
}

static int	get_word_len(char *in, int i)
{
	int	len;
	int	q;

	len = 0;
	q = 0;
	while (in[i + len])
	{
		if (!q && (in[i + len] == ' ' || in[i + len] == '\t'
				|| in[i + len] == '|' || in[i + len] == '<'
				|| in[i + len] == '>'))
			break ;
		if ((in[i + len] == '\'' || in[i + len] == '\"') && !q)
			q = in[i + len];
		else if (in[i + len] == q)
			q = 0;
		len++;
	}
	return (len);
}

t_token	*tokenize_input(char *in)
{
	t_token	*toks;
	char	*val;
	int		i;
	int		len;

	toks = NULL;
	i = 0;
	while (in[i])
	{
		i = skip_spaces(in, i);
		if (!in[i])
			break ;
		len = extract_operator(in, i, &toks);
		if (len == 0)
		{
			len = get_word_len(in, i);
			val = ft_substr(in, i, len);
			add_token(&toks, create_tok(val, CMD, 0));
			free(val);
		}
		i += len;
	}
	return (toks);
}
