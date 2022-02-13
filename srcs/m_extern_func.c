#include "../includes/minishell.h"

//잘못된 경로 접근 에러처리 함수
int	alert_path_error(char *cmd)
{
	ft_putstr_fd("minishell: ", STDOUT);
	ft_putstr_fd(cmd, STDOUT);
	ft_putendl_fd(": No such file or directory", STDOUT);
	g_exit = 127;
	return (0);
}

//경로와 실행명령문자열을 합쳐서 절대경로를 만드는 함수
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

//동적할당된 경로 목록을 해제하는 함수
void	paths_free(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i] != NULL)
		free(paths[i]);
	free(paths);
}

//매개변수로 받아온 경로들에 하나씩 stat함수로 실행되는지 테스트하는 함수
int	try_paths_exec(t_minishell *mini, char **paths, int *pipe_fd)
{
	struct stat	buf; //파일정보를 담는 구조체(dummy)
	int			i;
	char		*abs_path; //절대경로

	i = -1;
	while (paths[++i] != NULL)
	{ //경로에 실행할 파일을 이어 붙여서 절대경로 만듬 
		abs_path = trans_abs_path(mini, paths[i], mini->lo->cmdline[0].cmd);
		if (stat(abs_path, &buf) == 0) //만든 절대경로가 읽어지는 검사
		{
			system_call_exec(mini, abs_path, pipe_fd); //해당 절대경로로 실행하기
			free(abs_path);
			paths_free(paths);
			return (0);
		}
		free(abs_path);
	}
	paths_free(paths);
	return (alert_path_error(mini->lo->cmdline[0].cmd)); //경로들을 모두 돌아도 없다면 에러처리
}

//커스텀함수 이외의 명령를 실행하는 함수
int	extern_func_exec(t_minishell *mini, int *pipe_fd)
{
	struct stat	buf; //파일정보를 담는 구조체(dummy)
	int			path_index; //key환경변수의 PATH키의 index
	char		**paths; //PATH의 경로 목록들

	g_exit = 0;
	if (mini->lo->cmdline[0].cmd[0] == '\0')
		return (alert_path_error(""));
	if (stat(mini->lo->cmdline[0].cmd, &buf) == 0) //시스템콜 stat함수를 이용해서 파일이 읽어지는지 검사
		system_call_exec(mini, mini->lo->cmdline[0].cmd, pipe_fd); //읽어지면 실행 
	else //읽을 수 없다면 경로 목록에서 하나씩 해당경로에 실행할 파일이 있는지 검사
	{
		path_index = find_env(mini, "PATH"); //PATH의 인덱스 찾아서 변수에 할당
		if (path_index == -1) //PATH키가 없으면 에러
			return (alert_path_error(mini->lo->cmdline[0].cmd));
		paths = ft_split(mini->content[path_index], ':'); //2차원배열로 경로목록 만들기
		if (paths == NULL)
			exit_clean(mini, EXIT_FAILURE);
		return (try_paths_exec(mini, paths, pipe_fd)); //실행 시도해보기
	}
	return (0);
}
