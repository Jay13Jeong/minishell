#include "../includes/minishell.h"

//exit명령어시 개행을 제거하는 함수
void	show_exit_upline(void)
{
	char	*curr_path;
	char	*str_integer;

	curr_path = getcwd(0, 0);
	ft_putstr_fd("\x1b[1A\033[", STDOUT);
	str_integer = ft_itoa(ft_strlen(curr_path) + 10);
	ft_putstr_fd(str_integer, STDOUT);
	ft_putendl_fd("Cexit", STDOUT);
	free(curr_path);
	free(str_integer);
}

//현재경로를 표시하고 명령을 입력받는 함수
char	*readline_with_path(void)
{
	char	*curr_path;
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(SKY, "minish:");
	curr_path = getcwd(0, 0);
	temp2 = ft_strjoin(temp, curr_path);
	free(temp);
	free(curr_path);
	temp = ft_strjoin(temp2, " $ ");
	free(temp2);
	temp2 = ft_strjoin(temp, WHITE);
	free(temp);
	curr_path = readline(temp2);
	free(temp2);
	return (curr_path);
}

//환경변수 키의 중복을 검사하는 함수
int	check_unique_key(t_minishell *mini, char *new_key, char *new_value)
{
	int	same_key_idx;

	same_key_idx = find_env(mini, new_key); //환경변수에 키가 있다면 변수에 할당
	if (same_key_idx != -1) //만약 키가없어서 -1이 리턴되면 값을 새로운 값으로 바꿈
	{
		free(new_key);
		free(mini->content[same_key_idx]);
		mini->content[same_key_idx] = new_value;
		return (FAIL);
	}
	return (SUCCESS);
}
