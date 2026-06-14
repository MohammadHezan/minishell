/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaalrafa </var/spool/mail/zaalrafa>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 11:28:13 by zaalrafa          #+#    #+#             */
/*   Updated: 2026/06/14 11:46:58 by zaalrafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exit_no_args(t_cmd *cmd)
{
	long	code;

	code = cmd->shell->exit_status;
	free_shell(cmd->shell);
	exit((unsigned char)code);
}

int	skip_spaces_and_sign(char *s, int *sign)
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

int	check_overflow(char *s, int sign)
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
