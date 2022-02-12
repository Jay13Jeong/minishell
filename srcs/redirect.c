#include "../includes/minishell.h"

//리다이렉션 대상을 검사하는 함수
int	check_redirect_target(t_token *token)
{
	if (token->cmd == NULL) //그 대상의 문자열이 null이면 오류처리
	{
		alert_redirect_error("", 258);
		return (FAIL);
	}
	else if (token->redir_flag == 1) //리다이렉션 기호 뒤에 또 리다이렉션 기호가 있따면 오류처리
	{
		alert_redirect_error(token->cmd, 258);
		return (FAIL);
	}
	return (SUCCESS);
}

int	left_way_handler(t_minishell *mini, int i)
{
	int	oflag; //파일권한 번호
	int	test_fd; //테스트 할 일회성fd

	oflag = O_RDONLY; //읽기권한
	if (check_redirect_target(&mini->lo->cmdline[i + 1]) == FAIL) //리다이렉션 대상 검사
		return (FAIL);
	test_fd = -1;
	if (ft_strncmp("<", mini->lo->cmdline[i].cmd, 2) == 0) //리다이렉션 기호가 '<'일 때
		test_fd = open(mini->lo->cmdline[i + 1].cmd, oflag, 0644);
	else if (ft_strncmp("<<", mini->lo->cmdline[i].cmd, 3) == 0) //리다이렉션 기호가 '<<'일 때
		test_fd = 1; //(<<기호 미완성 부분 : 다중 << 기호 미구현) 
	if (test_fd < 1) //open이 안된다면 오류처리
	{
		alert_redirect_error(mini->lo->cmdline[i + 1].cmd, 1);
		return (FAIL);
	}
	close(test_fd);
	mini->lo->redirect_filename[0] = mini->lo->cmdline[i].cmd; //좌 기호 업데이트
	mini->lo->redirect_filename[1] = mini->lo->cmdline[i + 1].cmd; //좌 기호의 대상 업데이트
	return (SUCCESS);
}

//오른쪽 리다이렉션기호가 작동가능한지 테스트해보고 업데이트하는 함수
int	right_way_handler(t_minishell *mini, int i)
{
	int	oflag; //파일권한 번호
	int	test_fd; //테스트 할 일회성fd

	oflag = O_WRONLY | O_CREAT; //쓰기, 읽기권한
	if (check_redirect_target(&mini->lo->cmdline[i + 1]) == FAIL) //리다이렉션 대상 검사
		return (FAIL);
	test_fd = -1;
	if (ft_strncmp(">", mini->lo->cmdline[i].cmd, 2) == 0) //리다이렉션 기호가 '>'일 때
		test_fd = open(mini->lo->cmdline[i + 1].cmd, oflag | O_TRUNC, 0644);
	else if (ft_strncmp(">>", mini->lo->cmdline[i].cmd, 3) == 0) //리다이렉션 기호가 '>>'일 때
		test_fd = open(mini->lo->cmdline[i + 1].cmd, oflag | O_APPEND, 0644);
	if (test_fd < 1) //open 실패시 오류처리
	{
		alert_redirect_error(mini->lo->cmdline[i + 1].cmd, 1);
		return (FAIL);
	}
	close(test_fd);
	mini->lo->redirect_filename[2] = mini->lo->cmdline[i].cmd; //우 기호 업데이트
	mini->lo->redirect_filename[3] = mini->lo->cmdline[i + 1].cmd; //우 기호의 대상 업데이트
	return (SUCCESS);
}

//리다이렉션 기호의 방향을 검사하는 함수
int	way_check(t_token *tk, char c)
{
	if (c == '>') //오른쪽 맞는지
	{
		if (ft_strncmp(">", tk->cmd, 2) == 0)
			return (SUCCESS);
		if (ft_strncmp(">>", tk->cmd, 3) == 0)
			return (SUCCESS);
	}
	if (c == '<') //왼쪽 맞는지
	{
		if (ft_strncmp("<", tk->cmd, 2) == 0)
			return (SUCCESS);
		if (ft_strncmp("<<", tk->cmd, 3) == 0)
			return (SUCCESS);
	}
	return (FAIL);
}

//파이프를 이용해 리다이렉션 처리후 리다이렉션의 처리상태를 반환하는 함수
int	redirect_handler(t_minishell *mini, int **pipe_fd)
{
	int	i;

	i = -1;
	if (check_flag(mini) == FAIL) //오류가 있는 리다이렉션기호가 있다면 에러 반환
		return (ERROR);
	i = -1;
	while (++i < 4)
		mini->lo->redirect_filename[i] = NULL; //최종 리다이렉션 기호와 대상을 담을 배열을 초기화
	i = -1;
	while (mini->lo->cmdline[++i].cmd != NULL) 
	{
		if (mini->lo->cmdline[i].redir_flag == 0) //현재 토큰이 리다이렉션기호일 때 까지 찾는다.
			continue ;
		if (way_check(&mini->lo->cmdline[i], '<') == SUCCESS)  //현재 기호가 왼쪽 리다이렉션이면
			if (left_way_handler(mini, i) == FAIL)			//최종 기호 배열을 업데이트시도, 실패시 에러반환
				return (ERROR);
		if (way_check(&mini->lo->cmdline[i], '>') == SUCCESS) //현재 기호가 오른쪽 리다이렉션이면
			if (right_way_handler(mini, i) == FAIL)			//최종 기호 배열을 업데이트시도, 실패시 에러반환
				return (ERROR);
	}
	if (mini->lo->redirect_filename[0] || mini->lo->redirect_filename[2]) //리다이렉션기호가 배열에 좌우 하나라도 있다면
		return (do_redirect(mini->lo, pipe_fd));						//리다이렉션 처리해준다.
	return (0);
}
