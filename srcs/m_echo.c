#include "../includes/minishell.h"

void	write_args(t_token *token, int option_n, int fd_out)
{
	int	i;

	i = -1;
	while (1)
	{
		if (token[++i].cmd && token[i].redir_flag == 0)
		{
			if (i != 0)
				write(fd_out, " ", 1);
			ft_putstr_fd(token[i].cmd, fd_out);
		}
		else
		{
			if (option_n == 0)
				write(fd_out, "\n", 1);
			return ;
		}	
	}
}

int	mini_echo(t_minishell *mini, int fd_out)
{
	int	option_n;
	int	i;
	int	j;

	g_exit = 0;
	option_n = 0;
	i = 0;
	while (mini->lo->cmdline[++i].cmd)
	{
		if (mini->lo->cmdline[i].cmd[0] != '-')
			break ;
		if (mini->lo->cmdline[i].cmd[1] != 'n')
			break ;
		j = 0;
		while (mini->lo->cmdline[i].cmd[++j])
			if ((mini->lo->cmdline[i].cmd[j]) != 'n')
				break ;
		option_n = 1;
	}
	write_args(&mini->lo->cmdline[i], option_n, fd_out);
	return (0);
}
