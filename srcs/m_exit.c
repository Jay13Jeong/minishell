#include "../includes/minishell.h"

void	alert_exit_error(char *str, int exit_code)
{
	g_exit = exit_code;
	if (exit_code == 255)
	{
		ft_putstr_fd("minishell: exit: ", STDOUT);
		ft_putstr_fd(str, STDOUT);
		ft_putendl_fd(": numeric argument required", STDOUT);
	}
	else if (exit_code == 1)
	{
		ft_putstr_fd("minishell: exit: ", STDOUT);
		ft_putendl_fd("too many arguments", STDOUT);
	}
}

void	exit_clean(t_minishell *mini, int error_code)
{
	free_all_list(mini->lo);
	free_key_value(mini);
	exit(error_code);
}

void	exit_minishell(t_minishell *mini)
{
	int	i;

	ft_putendl_fd("exit", STDOUT_BACKUP);
	if (mini->lo->cmdline[1].cmd == NULL)
		exit_clean(mini, 0);
	i = -1;
	while (mini->lo->cmdline[1].cmd[++i])
	{
		if (!ft_isdigit(mini->lo->cmdline[1].cmd[i]))
		{
			alert_exit_error(mini->lo->cmdline[1].cmd, 255);
			exit_clean(mini, 255);
		}
	}
	if (mini->lo->cmdline[2].cmd != NULL)
	{
		alert_exit_error(mini->lo->cmdline[1].cmd, 1);
		exit_clean(mini, 1);
	}
	exit_clean(mini, ft_atoi(mini->lo->cmdline[1].cmd) % 256);
}

int	mini_exit(t_minishell *mini)
{
	int	i;

	if (mini->lo->exit_flag == 1)
		exit_minishell(mini);
	if (mini->lo->cmdline[1].cmd == NULL)
	{
		g_exit = 0;
		return (0);
	}
	i = -1;
	while (mini->lo->cmdline[1].cmd[++i])
	{
		if (!ft_isdigit(mini->lo->cmdline[1].cmd[i]))
		{
			alert_exit_error(mini->lo->cmdline[1].cmd, 255);
			return (0);
		}
	}
	if (mini->lo->cmdline[2].cmd != NULL)
	{
		alert_exit_error(mini->lo->cmdline[1].cmd, 1);
		return (0);
	}
	g_exit = ft_atoi(mini->lo->cmdline[1].cmd) % 256;
	return (0);
}
