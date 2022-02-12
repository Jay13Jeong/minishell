#include "../includes/minishell.h"

void	alert_extern_error(char *cmd, char *key)
{
	ft_putstr_fd("minishell: ", STDOUT);
	ft_putstr_fd(cmd, STDOUT);
	ft_putstr_fd(": `", STDOUT);
	ft_putstr_fd(key, STDOUT);
	ft_putendl_fd("': not a valid identifier", STDOUT);
	g_exit = 1;
}

int	count_args(t_token *token)
{
	int			i;

	i = 0;
	while (token[i].cmd != NULL && token[i].redir_flag == 0)
		i++;
	return (i);
}

void	exit_process(t_minishell *mini, char **extern_argv, int exit_code)
{
	free(extern_argv);
	exit_clean(mini, exit_code);
}

void	wait_child_process(pid_t child_pid)
{
	int			status;

	waitpid(child_pid, &status, 0);
	g_exit = status >> 8;
}

void	system_call_exec(t_minishell *mini, char *path, int *fds)
{
	pid_t	pid;
	int		i;
	int		extern_len;
	char	**extern_argv;

	pid = fork();
	if (pid == -1)
		exit_clean(mini, EXIT_FAILURE);
	if (pid == 0)
	{
		extern_len = count_args(mini->lo->cmdline);
		extern_argv = (char **)malloc(sizeof(char *) * (extern_len + 1));
		extern_argv[0] = path;
		i = 0;
		while (++i < extern_len)
			extern_argv[i] = mini->lo->cmdline[i].cmd;
		extern_argv[i] = NULL;
		if (mini->lo->pipe_flag == 1 && mini->lo->right_flag == 0)
			dup2(fds[1], 1);
		if (execve(path, extern_argv, mini->env) == -1)
			exit_process(mini, extern_argv, EXIT_FAILURE);
		exit_process(mini, extern_argv, EXIT_SUCCESS);
	}
	wait_child_process(pid);
}
