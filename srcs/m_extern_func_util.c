#include "../includes/minishell.h"

// void	alert_extern_error(char *cmd, char *key)
// {
// 	ft_putstr_fd("minishell: ", STDOUT);
// 	ft_putstr_fd(cmd, STDOUT);
// 	ft_putstr_fd(": `", STDOUT);
// 	ft_putstr_fd(key, STDOUT);
// 	ft_putendl_fd("': not a valid identifier", STDOUT);
// 	g_exit = 1;
// }

//실행 할 명령의 인자의 수를 카운트하는 함수
int	count_args(t_token *token)
{
	int			i;

	i = 0;
	while (token[i].cmd != NULL && token[i].redir_flag == 0)
		i++;
	return (i);
}

//동적할당 된 인자목록을 해제하고 실행상태를 exit함수로 반환하면서 종료 
void	exit_process(t_minishell *mini, char **extern_argv, int exit_code)
{
	free(extern_argv);
	exit_clean(mini, exit_code);
}

//매개변수의 자식프로세서가 exit될 때 까지 기다리고있다가 종료되면 그 상태를 전역변수에 업데이트하는 함수  
void	wait_child_process(pid_t child_pid)
{
	int			status;

	waitpid(child_pid, &status, 0);
	g_exit = status >> 8; //status는 16비트로 이루어져있음 exit함수로 종료되었다면 함수의 매개변수가 앞의 8비트에 담김
}

//매개변수로 받은 경로의 명령을 실행하는 함수
void	system_call_exec(t_minishell *mini, char *path, int *fds)
{
	pid_t	pid;
	int		i;
	int		extern_len;
	char	**extern_argv;

	pid = fork(); //자식 프로세서 만들기 
	if (pid == -1) //생성 오류 처리
		exit_clean(mini, EXIT_FAILURE);
	if (pid == 0) //자식프로세서 일 떄
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
	wait_child_process(pid); //부모프로세서는 자식을 기다린다.
}
