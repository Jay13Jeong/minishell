#include "../includes/minishell.h"

//파이프 에러를 처리하는 함수
int	exit_pipe_error(void)
{
	ft_putstr_fd("minishell: ", STDOUT);
	ft_putendl_fd("syntax error near unexpected token `|'", STDOUT);
	g_exit = 258;
	return (ERROR);
}

//리다이렉션 설정을 시작하는 함수
int	set_redirect(t_minishell *mini, int *fd_out, int **pipe_fd)
{
	mini->lo->right_flag = redirect_handler(mini, pipe_fd); //리다이렉션 처리후 리다이렉션의 상태를 대입
	if (mini->lo->right_flag == ERROR) //리다이렉션 오류시 에러 반환한다. 
		return (ERROR);
	if (mini->lo->pipe_flag == 1 && mini->lo->right_flag == 0) //다음 명령리스트가 있고 오른쪽 리다이렉션이면,
		*fd_out = (*pipe_fd)[1];					//현재링크에서 출력되는 문자열들은 모두 파이프로 보내도록 길을 바꿔준다.
	else
		*fd_out = STDOUT;	//조건에 맞지않으면 모든 문자열들은 표준 출력하도록 길을 설정
	return (SUCCESS);
}

//리다이렉션 설정후 커스텀 된 함수를 우선순위로 실행하고 없다면 os에 있는 함수를 실행하는 함수 
int	func_exec(t_minishell *mini, int *pipe_fd)
{
	int			fd_out;

	if (mini->lo->cmdline[0].cmd == NULL) //현재 파이프 구간에 명령이 없다면 에러처리한다
		return (exit_pipe_error()); 
	if (set_redirect(mini, &fd_out, &pipe_fd) == ERROR) //리다이렉션 설정, 실패시 에러 반환
		return (ERROR);
	if (ft_strncmp("echo", mini->lo->cmdline[0].cmd, 5) == 0) //여기서부터 실행 할 명령이 커스텀 함수인지 검사한다. 
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
	return (extern_func_exec(mini, pipe_fd)); //실행 할 명령이 커스텀 함수 중에 없다면, os에 있는 함수를 실행한다.
}

//프로세를 생성해서 다음 명령을 실행하는 함수
int	separate_proc(t_minishell *mini, int *pipe_fd)
{
	pid_t	pid;
	int		status;

	if (mini->lo->pipe_flag != 1) //만약 다음명령이 없다면 종료
		return (g_exit);
	close(pipe_fd[1]); //파이프의 입구 초기화
	pid = fork(); //프로세스 생성
	if (pid > 0) //부모 프로세스 일때
	{
		close(pipe_fd[0]); //파이프 출구 초기화
		waitpid(pid, &status, 0); //자식프로세스가 종료까지 기다림
		return (status >> 8); //자식종료상태를 반환
	}
	else if (pid == 0) //자식 프로세스일 때
	{
		dup2(pipe_fd[0], STDIN); //파이프의 출구fd를 0으로 초기화
		close(pipe_fd[0]); //초기화후 기존 fd 닫기
		free_linked_order(mini); //사용이 끝난 기존 명령은 폐기한다
		g_exit = exec(mini); //명령을 실행후 반환값을 전역변수를 업데이트해준다 (재귀)
		exit(g_exit); //자식프로세스 종료
	}
	return (g_exit);
}

int	exec(t_minishell *mini)
{
	int	pipe_fd[2];

	pipe(pipe_fd); //파이프를 생성
	if (func_exec(mini, pipe_fd) != 0) //명령실행 후 최종반환 될 값이 0이 아니면 그 상태를 바로 리턴
		return (g_exit);
	dup2(STDIN_BACKUP, STDIN); //변경되었던 표준입력fd를 백업fd를 통해 되돌린다.
	dup2(STDOUT_BACKUP, STDOUT); //변경되었던 표준출력fd를 백업fd를 통해 되돌린다.
	return (separate_proc(mini, pipe_fd)); //다음 명령은 새로운 프로세스를 생성해서 실행한다.
}
