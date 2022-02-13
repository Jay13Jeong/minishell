#include "../includes/minishell.h"

//env
int	mini_env(t_minishell *mini, int fd_out)
{
	int	i;

	i = 0;
	while (i < mini->len) //환경변수 목록 출력
	{
		ft_putstr_fd(mini->key[i], fd_out);
		ft_putchar_fd('=', fd_out);
		ft_putendl_fd(mini->content[i], fd_out);
		i++;
	}
	g_exit = 0;
	return (0);
}
