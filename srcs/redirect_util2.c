#include "../includes/minishell.h"

int	check_flag(t_minishell *mini)
{
	int	i;

	mini->lo->right_flag = 0;
	i = -1;
	while (mini->lo->cmdline[++i].cmd)
	{
		if (mini->lo->cmdline[i].redir_flag == -1)
		{
			alert_redirect_error(mini->lo->cmdline[i].cmd, 258);
			return (FAIL);
		}
	}
	return (SUCCESS);
}

void	alert_redirect_error(char *str, int exit_code)
{
	g_exit = exit_code;
	if (str == NULL)
		str = "";
	if (exit_code == 1)
	{
		ft_putstr_fd("minishell: ", STDOUT);
		ft_putstr_fd(str, STDOUT);
		ft_putendl_fd(": No such file or directory", STDOUT);
	}
	else if (exit_code == 258)
	{
		ft_putstr_fd("minishell: ", STDOUT);
		ft_putstr_fd("syntax error near unexpected token `", STDOUT);
		ft_putstr_fd(str, STDOUT);
		ft_putendl_fd("'", STDOUT);
	}
}
