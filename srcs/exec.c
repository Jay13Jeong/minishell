#include "../includes/minishell.h"

int	exit_pipe_error(void)
{
	ft_putstr_fd("minishell: ", STDOUT);
	ft_putendl_fd("syntax error near unexpected token `|'", STDOUT);
	g_exit = 258;
	return (ERROR);
}

int	set_redirect(t_minishell *mini, int *fd_out, int **pipe_fd)
{
	mini->lo->right_flag = redirect_handler(mini, pipe_fd);
	if (mini->lo->right_flag == ERROR)
		return (ERROR);
	if (mini->lo->pipe_flag == 1 && mini->lo->right_flag == 0)
		*fd_out = (*pipe_fd)[1];
	else
		*fd_out = STDOUT;
	return (SUCCESS);
}

int	func_exec(t_minishell *mini, int *pipe_fd)
{
	int			fd_out;

	if (mini->lo->cmdline[0].cmd == NULL)
		return (exit_pipe_error());
	if (set_redirect(mini, &fd_out, &pipe_fd) == ERROR)
		return (ERROR);
	if (ft_strncmp("echo", mini->lo->cmdline[0].cmd, 5) == 0)
		return (mini_echo(mini, fd_out));
	else if (ft_strncmp("cd", mini->lo->cmdline[0].cmd, 3) == 0)
		return (mini_cd(mini));
	else if (ft_strncmp("pwd", mini->lo->cmdline[0].cmd, 4) == 0)
		return (mini_pwd(fd_out));
	else if (ft_strncmp("export", mini->lo->cmdline[0].cmd, 7) == 0)
		return (mini_export(mini, fd_out));
	else if (ft_strncmp("unset", mini->lo->cmdline[0].cmd, 6) == 0)
		return (mini_unset(mini));
	else if (ft_strncmp("env", mini->lo->cmdline[0].cmd, 4) == 0)
		return (mini_env(mini, fd_out));
	else if (ft_strncmp("exit", mini->lo->cmdline[0].cmd, 5) == 0)
		return (mini_exit(mini));
	return (extern_func_exec(mini, pipe_fd));
}

int	separate_proc(t_minishell *mini, int *pipe_fd)
{
	pid_t	pid;
	int		status;

	if (mini->lo->pipe_flag != 1)
		return (g_exit);
	close(pipe_fd[1]);
	pid = fork();
	if (pid > 0)
	{
		close(pipe_fd[0]);
		waitpid(pid, &status, 0);
		return (status >> 8);
	}
	else if (pid == 0)
	{
		dup2(pipe_fd[0], STDIN);
		close(pipe_fd[0]);
		free_linked_order(mini);
		g_exit = exec(mini);
		exit(g_exit);
	}
	return (g_exit);
}

int	exec(t_minishell *mini)
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	if (func_exec(mini, pipe_fd) != 0)
		return (g_exit);
	dup2(STDIN_BACKUP, STDIN);
	dup2(STDOUT_BACKUP, STDOUT);
	return (separate_proc(mini, pipe_fd));
}
