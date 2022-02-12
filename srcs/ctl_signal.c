#include "../includes/minishell.h"

//컨트롤키 + c 의 행동을 처리하는 함수
void	ctl_c(int sig)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG); //현재 프로세스의 자식상태를 즉시 반환
	if (pid == -1) //자식프로세스가 없다면
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \n", STDOUT);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else //자식프로세스가 있다면
		ft_putstr_fd("\n", STDOUT);
	sig = 0;
}

//컨트롤키 + \ 의 행동을 처리하는 함수
void	ctl_back_slash(int sig)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG); //현재프로세스의 자식상태를 즉시반환
	if (pid == -1)//자식 프로세스가 없다면
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b", STDOUT);
	}
	else //자식프로세스가 있다면 
		ft_putstr_fd("Quit: 3\n", STDOUT);
	sig = 0;
}

//시그널의 행동을 수정하는 함수
void	init_signal(void)
{
	signal(SIGINT, ctl_c);
	signal(SIGQUIT, ctl_back_slash);
}
