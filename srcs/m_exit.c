#include "../includes/minishell.h"

//exit함수의 오류를 처리하는 함수
void	alert_exit_error(char *str, int exit_code)
{
	g_exit = exit_code;
	if (exit_code == 255)
	{
		ft_putstr_fd("minishell: exit: ", STDOUT);
		ft_putstr_fd(str, STDOUT);
		ft_putendl_fd(": numeric argument required", STDOUT);
	}
	else if (exit_code == 1)
	{
		ft_putstr_fd("minishell: exit: ", STDOUT);
		ft_putendl_fd("too many arguments", STDOUT);
	}
}

//동적할당된 링크드 리스트와 환경변수의 메모리를 모두 해제
void	exit_clean(t_minishell *mini, int error_code)
{
	free_all_list(mini->lo);
	free_key_value(mini);
	exit(error_code);
}

//쉘을 종료하는 함수
void	exit_minishell(t_minishell *mini)
{
	int	i;

	ft_putendl_fd("exit", STDOUT_BACKUP);
	if (mini->lo->cmdline[1].cmd == NULL) //매개변수가 없다면
		exit_clean(mini, 0);
	i = -1;
	while (mini->lo->cmdline[1].cmd[++i]) //매개변수가 문자일 때
	{
		if (!ft_isdigit(mini->lo->cmdline[1].cmd[i]))
		{
			alert_exit_error(mini->lo->cmdline[1].cmd, 255);
			exit_clean(mini, 255);
		}
	}
	if (mini->lo->cmdline[2].cmd != NULL) //첫 매개변수는 숫자지만 또 다른 매개변수가 존재하면
	{
		alert_exit_error(mini->lo->cmdline[1].cmd, 1);
		exit_clean(mini, 1);
	}
	exit_clean(mini, ft_atoi(mini->lo->cmdline[1].cmd) % 256); //매개변수를 256나눈 나머지
}

//exit
int	mini_exit(t_minishell *mini)
{
	int	i;

	if (mini->lo->exit_flag == 1) //현재 프로세서가 첫 프로세서면 즉시 쉘종료 
		exit_minishell(mini); 
	if (mini->lo->cmdline[1].cmd == NULL) //매개변수가 없다면
	{
		g_exit = 0;
		return (0);
	}
	i = -1;
	while (mini->lo->cmdline[1].cmd[++i]) //매개변수가 문자일 때
	{
		if (!ft_isdigit(mini->lo->cmdline[1].cmd[i]))
		{
			alert_exit_error(mini->lo->cmdline[1].cmd, 255);
			return (0);
		}
	}
	if (mini->lo->cmdline[2].cmd != NULL) //첫 매개변수는 숫자지만 또 다른 매개변수가 존재하면
	{
		alert_exit_error(mini->lo->cmdline[1].cmd, 1);
		return (0);
	}
	g_exit = ft_atoi(mini->lo->cmdline[1].cmd) % 256; //매개변수를 256나눈 나머지
	return (0);
}
