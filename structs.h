/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:13:07 by mohammad-he       #+#    #+#             */
/*   Updated: 2026/05/19 10:14:28 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>

typedef struct s_shell	t_shell;
typedef enum e_token_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	CMD,
	TRUNCATE,
	APPEND,
	HEREDOC,
	PIPE,
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	int					quote_type;
	struct s_token		*next;
}						t_token;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_cmd
{
	char				**args;
	int					infile;
	int					outfile;
	bool				append;
	bool				pipe;
	char				*limiter;
	struct s_cmd		*next;
	t_shell				*shell;
}						t_cmd;

typedef struct s_shell
{
	char				**history;
	pid_t				*pids;
	int					stdin_backup;
	int					stdout_backup;
	bool				env_edited;
	char				**env_array;
	struct s_env		*env;
	struct s_cmd		*current_cmd;
	struct s_token		*tokens;
	int					exit_status;
}						t_shell;
