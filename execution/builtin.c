/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa <zaalrafa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 13:29:45 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/07 14:19:10 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->env_edited)
	{
		free(shell->env_array);
		shell->env_array = rebuild_env(shell);
	}
	while (shell->env_array[i])
	{
		if (ft_strchr(shell->env_array[i], '='))
			printf("%s\n", shell->env_array[i]);
		i++;
	}
	shell->exit_status = 0;
}

void	ft_cd(t_cmd *cmd)
{
	char	*path;
	char	cwd[4096];
	char	*oldpwd;

	if (cmd->args[1] != NULL && cmd->args[2] != NULL)
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)))
		oldpwd = ft_strdup(cwd);
	else
		oldpwd = ft_strdup("");
	if (cmd->args[1] == NULL)
		path = get_env_value(cmd->shell->env, "HOME");
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		path = get_env_value(cmd->shell->env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			free(oldpwd);
			cmd->shell->exit_status = 1;
			return ;
		}
		ft_putendl_fd(path, 1);
	}
	else
		path = cmd->args[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		free(oldpwd);
		cmd->shell->exit_status = 1;
		return ;
	}
	cmd->shell->exit_status = 0;
	change_env_value(cmd->shell->env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		change_env_value(cmd->shell->env, "PWD", ft_strdup(cwd));
}

static void	exit_no_args(t_cmd *cmd)
{
	long	code;

	code = cmd->shell->exit_status;
	free_shell(cmd->shell);
	exit((unsigned char)code);
}

static int	skip_spaces_and_sign(char *s, int *sign)
{
	int	i;

	i = 0;
	*sign = 1;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

static int	check_overflow(char *s, int sign)
{
	int		len;
	char	*max;

	while (*s == '0')
		s++;
	len = 0;
	while (s[len] >= '0' && s[len] <= '9')
		len++;
	if (len > 19)
		return (0);
	if (len < 19)
		return (1);
	if (sign == 1)
		max = "9223372036854775807";
	else
		max = "9223372036854775808";
	if (ft_strncmp(s, max, 19) > 0)
		return (0);
	return (1);
}

int	is_exit_numeric(char *s)
{
	int	i;
	int	sign;

	i = skip_spaces_and_sign(s, &sign);
	if (!s[i] || !ft_isdigit(s[i]))
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	i = skip_spaces_and_sign(s, &sign);
	return (check_overflow(s + i, sign));
}

static void	exit_error(t_cmd *cmd, char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	free_shell(cmd->shell);
	exit(2);
}

void	ft_exit(t_cmd *cmd)
{
	long long	code;
	char		*arg;
	int			i;

	i = 1;
	if (cmd->args[i] && ft_strcmp(cmd->args[i], "--") == 0)
		i++;
	if (!cmd->args[i])
		exit_no_args(cmd);
	arg = cmd->args[i];
	if (!is_exit_numeric(arg))
		exit_error(cmd, arg);
	code = ft_atoll(arg);
	if (cmd->args[i + 1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	ft_putendl_fd("exit", 2);
	free_shell(cmd->shell);
	exit((unsigned char)code);
}

void	ft_pwd(t_shell *shell)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)))
		{printf("%s\n", cwd);
			shell->exit_status = 0;
		}
	else
		perror("minishell: pwd");
}
