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

# include "42_libft/libft.h"
# include "structs.h"
# include "42_gnl/get_next_line_bonus.h"
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

//------------utils--------------//
void	free_arr(char **arr);
void	error_message(char *str, int errno);
int		count_env(t_env *env);
void	free_env_node(t_env *tmp);

//------------execution----------//
char	*cmd_path(t_shell *shell, char *cmd);
char	*check_path(t_shell *shell, char *cmd);
char	**rebuild_env(t_shell *shell);
bool	change_env_value(t_env *env, char *key, char *value);
char	*get_env_value(t_env *env, char *key);
t_env	*get_env_by_key(t_env *env, char *key);
//--builtin--//
void	ft_env(t_shell *shell);
void	ft_unset(t_shell *shell, t_cmd *cmd);
void	ft_export(t_shell *shell, t_cmd *cmd);
void	ft_exit(t_shell *shell);
void	ft_pwd(void);
void	ft_cd(t_cmd *cmd);

//------------parsing------------//
#endif
