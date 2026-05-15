/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:13:41 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/12 11:18:58 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_quotes(char *s, int i, t_token **toks)
{
	int		len;
	char	q;
	int		qt;

	q = s[i];
	len = 1;
	while (s[i + len] && s[i + len] != q)
		len++;
	if (s[i + len] == q)
		len++;
	if (q == '\'')
		qt = 1;
	else
		qt = 2;
	add_token(toks, create_tok(ft_substr(s, i, len), CMD, qt));
	return (len);
}

static int	handle_operator(char *s, int i, t_token **toks)
{
	if (!ft_strncmp(&s[i], "<<", 2))
		return (add_token(toks, create_tok(ft_strdup("<<"), HEREDOC, 0)) + 1);
	if (!ft_strncmp(&s[i], ">>", 2))
		return (add_token(toks, create_tok(ft_strdup(">>"), APPEND, 0)) + 1);
	if (s[i] == '<')
		return (add_token(toks, create_tok(ft_strdup("<"), REDIRECT_IN, 0)));
	if (s[i] == '>')
		return (add_token(toks, create_tok(ft_strdup(">"), REDIRECT_OUT, 0)));
	if (s[i] == '|')
		return (add_token(toks, create_tok(ft_strdup("|"), PIPE, 0)));
	return (0);
}

static int	handle_word(char *s, int i, t_token **toks)
{
	int		len;
	char	*val;

	len = 0;
	while (s[i + len] && s[i + len] != ' ' && s[i + len] != '\t'
		&& s[i + len] != '|' && s[i + len] != '<' && s[i + len] != '>'
		&& s[i + len] != '\'' && s[i + len] != '\"')
		len++;
	val = ft_substr(s, i, len);
	add_token(toks, create_tok(val, CMD, 0));
	return (len);
}

t_token	*tokenize_input(char *in)
{
	t_token	*toks;
	int		i;

	toks = NULL;
	i = 0;
	while (in[i])
	{
		while (in[i] == ' ' || in[i] == '\t')
			i++;
		if (!in[i])
			break ;
		if (in[i] == '\'' || in[i] == '\"')
			i += handle_quotes(in, i, &toks);
		else if (in[i] == '|' || in[i] == '<' || in[i] == '>')
			i += handle_operator(in, i, &toks);
		else
			i += handle_word(in, i, &toks);
	}
	return (toks);
}
