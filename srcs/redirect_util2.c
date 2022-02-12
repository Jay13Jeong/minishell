#include "../includes/minishell.h"

//오류가 있는 리다이렉션기호있는지 검사하는 함수
int	check_flag(t_minishell *mini)
{
	int	i;

	mini->lo->right_flag = 0; //리다이렉션 상태를 초기화 (파싱쪽 함수로 코드를 옮길 예정)
	i = -1;
	while (mini->lo->cmdline[++i].cmd)
	{
		if (mini->lo->cmdline[i].redir_flag == -1) //에러가 있는 기호가 있다면 오류처리
		{
			alert_redirect_error(mini->lo->cmdline[i].cmd, 258);
			return (FAIL);
		}
	}
	return (SUCCESS);
}

//리다이렉션의 오류를 처리하는 함수
void	alert_redirect_error(char *str, int exit_code)
{
	g_exit = exit_code;
	if (str == NULL)
		str = "";
	if (exit_code == 1) //잘못된 대상 오류
	{
		ft_putstr_fd("minishell: ", STDOUT);
		ft_putstr_fd(str, STDOUT);
		ft_putendl_fd(": No such file or directory", STDOUT);
	}
	else if (exit_code == 258)//기호가 규칙에 맞지 않을 떄 오류
	{
		ft_putstr_fd("minishell: ", STDOUT);
		ft_putstr_fd("syntax error near unexpected token `", STDOUT);
		ft_putstr_fd(str, STDOUT);
		ft_putendl_fd("'", STDOUT);
	}
}
