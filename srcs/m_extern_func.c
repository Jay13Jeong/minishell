#include "../includes/minishell.h"

int	alert_path_error(char *cmd)
{
	ft_putstr_fd("minishell: ", STDOUT);
	ft_putstr_fd(cmd, STDOUT);
	ft_putendl_fd(": No such file or directory", STDOUT);
	g_exit = 127;
	return (0);
}

char	*trans_abs_path(t_minishell *mini, char *path, char *cmd)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(path, "/");
	if (temp == NULL)
		exit_clean(mini, EXIT_FAILURE);
	temp2 = ft_strjoin(temp, cmd);
	free(temp);
	if (temp2 == NULL)
		exit_clean(mini, EXIT_FAILURE);
	return (temp2);
}

void	paths_free(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i] != NULL)
		free(paths[i]);
	free(paths);
}

int	try_paths_exec(t_minishell *mini, char **paths, int *pipe_fd)
{
	struct stat	buf;
	int			i;
	char		*abs_path;

	i = -1;
	while (paths[++i] != NULL)
	{
		abs_path = trans_abs_path(mini, paths[i], mini->lo->cmdline[0].cmd);
		if (stat(abs_path, &buf) == 0)
		{
			system_call_exec(mini, abs_path, pipe_fd);
			free(abs_path);
			paths_free(paths);
			return (0);
		}
		free(abs_path);
	}
	paths_free(paths);
	return (alert_path_error(mini->lo->cmdline[0].cmd));
}

int	extern_func_exec(t_minishell *mini, int *pipe_fd)
{
	struct stat	buf;
	char		**paths;
	int			path_index;

	g_exit = 0;
	if (mini->lo->cmdline[0].cmd[0] == '\0')
		return (alert_path_error(""));
	if (stat(mini->lo->cmdline[0].cmd, &buf) == 0)
		system_call_exec(mini, mini->lo->cmdline[0].cmd, pipe_fd);
	else
	{
		path_index = find_env(mini, "PATH");
		if (path_index == -1)
			return (alert_path_error(mini->lo->cmdline[0].cmd));
		paths = ft_split(mini->content[path_index], ':');
		if (paths == NULL)
			exit_clean(mini, EXIT_FAILURE);
		return (try_paths_exec(mini, paths, pipe_fd));
	}
	return (0);
}
