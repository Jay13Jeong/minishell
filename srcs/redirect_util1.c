#include "../includes/minishell.h"

//'<' 기호를 적용하는 함수
int	single_left_redirect(t_linked_order *order)
{
	int			file_fd; //리다이렉션 대상 파일 fd 
	int			oflag; //파일권한

	oflag = O_RDONLY;
	file_fd = open(order->redirect_filename[1], oflag, 0644);
	if (file_fd < 1)
	{
		alert_redirect_error(order->redirect_filename[1], 1);
		return (ERROR);
	}
	dup2(file_fd, STDIN); //정상적으로 fd가 열리면, fd가 가르키는 파일의 내용을 표준입력fd로 연결해준다
	close(file_fd);
	return (0);
}

//'<<' 기호를 적용하는 함수
int	double_left_redirect(t_linked_order *order, int **pipe_fd)
{
	char		*line; //텍스트를 입력받을 변수
	char		*syntax; //구분자 문자열
	int			syn_len; //구분자 문자열 길이

	syntax = order->redirect_filename[1];
	syn_len = ft_strlen(syntax);
	while (1)
	{
		line = readline("> "); //readline함수로 텍스트 입력받기
		if (ft_strncmp(line, syntax, syn_len + 1) == 0) //구분자를 만나면 입력 종료
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, (*pipe_fd)[1]); //파이프 입구fd에 입력받은 텍스트를 보낸다.
		free(line);
	}
	dup2((*pipe_fd)[0], STDIN); //파이프 출구fd를 표준입력fd로 연결, 이제 파이프로 보낸 내용을 입력받은 텍스트처럼 사용가능 
	close((*pipe_fd)[0]); //사용이 끝난 파이프를 초기화 해준다.
	close((*pipe_fd)[1]);
	pipe(*pipe_fd);
	return (0);
}

//'>' 기호를 적용하는 함수
int	single_right_redirect(t_linked_order *order)
{
	int			file_fd;
	int			oflag; //권한

	oflag = O_WRONLY | O_CREAT | O_TRUNC; //쓰기, 없으면 만들기, 있으면 내용비우기
	file_fd = open(order->redirect_filename[3], oflag, 0644);
	if (file_fd < 1)
	{
		alert_redirect_error(order->redirect_filename[3], 1);
		return (ERROR);
	}
	dup2(file_fd, STDOUT); //앞으로 출력결과물을 이 곳으로 보내도록 길 변경 
	close(file_fd);
	return (1);
}

//'>>' 기호를 적용하는 함수
int	double_right_redirect(t_linked_order *order)
{
	int			file_fd;
	int			oflag; //권한

	oflag = O_WRONLY | O_CREAT | O_APPEND; //쓰기, 없으면 만들기, 있으면 개행하고 이어서 작성
	file_fd = open(order->redirect_filename[3], oflag, 0644);
	if (file_fd < 1)
	{
		alert_redirect_error(order->redirect_filename[3], 1);
		return (ERROR);
	}
	dup2(file_fd, STDOUT); //앞으로 출력결과물을 이 곳으로 보내도록 길 변경
	close(file_fd);
	return (1);
}

//최종적으로 업데이트된 리다이렉션 기호를 적용하는 함수
int	do_redirect(t_linked_order *order, int **pipe_fd)
{
	int	left_status; //좌 리다이렉션 상태
	int	right_status; //우 리다이렉션 상태

	left_status = 0; //초기화
	right_status = 0;
	if (ft_strncmp("<", order->redirect_filename[0], 2) == 0) //배열에 들어있는 기호에 맞는 함수를 실행해준다.
		left_status = single_left_redirect(order);
	else if (ft_strncmp("<<", order->redirect_filename[0], 3) == 0)
		left_status = double_left_redirect(order, pipe_fd);
	if (ft_strncmp(">", order->redirect_filename[2], 2) == 0)
		right_status = single_right_redirect(order);
	else if (ft_strncmp(">>", order->redirect_filename[2], 3) == 0)
		right_status = double_right_redirect(order);
	if (left_status == -1 || right_status == -1) //적용 실패하면 에러 반환
		return (ERROR);
	return (right_status == 1); //오른쪽 리다이렉션이 있다면 1 아니면 0 반환
}
