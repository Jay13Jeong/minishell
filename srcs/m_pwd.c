#include "../includes/minishell.h"

//pwd
int	mini_pwd(int fd_out)
{
	char	*path;

	path = getcwd(NULL, 0); /*현재 경로 표시 */
	ft_putendl_fd(path, fd_out); 
	free(path);
	g_exit = 0;
	return (0);
}
