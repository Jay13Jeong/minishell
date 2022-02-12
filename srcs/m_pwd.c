#include "../includes/minishell.h"

int	mini_pwd(int fd_out)
{
	char	*path;

	path = getcwd(NULL, 0);
	ft_putendl_fd(path, fd_out);
	free(path);
	g_exit = 0;
	return (0);
}
