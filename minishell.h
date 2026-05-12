/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 15:36:41 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/05/12 11:34:30 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "structs.h"
# include "get_next_lien/get_next_line_bonus.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

t_token	*create_tok(char *value, t_token_type type, int quote);
int		add_token(t_token **tokens, t_token *new_node);
t_token	*tokenize_input(char *in);
bool	check_syntax(t_token *tokens);
void	expand_tokens(t_shell *shell);
t_cmd	*create_cmd_node(void);
void	add_cmd(t_cmd **cmds, t_cmd *new_cmd);
t_cmd	*build_cmd_table(t_token *tokens);
t_env	*init_env(char **envp);
bool	parse_input(t_shell *shell, char *input);
void	execute_commands(t_shell *shell);
void	free_cycle(t_shell *shell);
void	free_shell(t_shell *shell);
void	init_shell(t_shell *shell, char **envp);
void	handle_redirection(t_cmd *cmd, t_token **tok);

#endif
