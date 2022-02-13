#include "../includes/minishell.h"

//기존 목록보다 크기가 +1된 2차원 배열을 동적할당하는 함수
void	make_new_export_space(t_minishell *mini, char ***temp, char ***temp2)
{
	*temp = (char **)malloc(sizeof(char *) * (mini->len + 2)); //기존 목록 길이 + 1 + null(1)
	if (*temp == NULL)
		exit_clean(mini, EXIT_FAILURE);
	*temp2 = (char **)malloc(sizeof(char *) * (mini->len + 2));
	if (*temp2 == NULL)
	{
		free(*temp);
		exit_clean(mini, EXIT_FAILURE);
	}
}

//환경변수에 키,값을 등록하는 함수
void	enrol_env(t_minishell *mini, char *new_key, char *new_value)
{
	char	**temp;
	char	**temp2;
	int		i;

	if (check_unique_key(mini, new_key, new_value) == FAIL) //키가 이미 있는지 확인
		return ;
	make_new_export_space(mini, &temp, &temp2); //추가된 크기만큼 담을 환경변수의 변수를 새로 동적할당
	i = -1;
	while (++i < mini->len) //기존목록을 새 목록으로 이동
	{
		temp[i] = mini->key[i];
		temp2[i] = mini->content[i];
	}
	temp[i] = new_key; /*마지막에 새로 추가되는 환경변수를 추가하고 환경변수의 길이를 +1한다 */ 
	temp2[i++] = new_value;
	temp[i] = NULL;
	temp2[i] = NULL;
	free(mini->key);
	free(mini->content);
	mini->key = temp;
	mini->content = temp2;
	mini->len += 1;
}

//환경변수 목록에 새로운 환경변수를 추가하는 함수
void	add_path(t_minishell *mini, char *str, char *chr)
{
	char	*key;
	char	*value;
	char	**arr;

	if (key_check_str(str, '=') == FAIL) //key형식 검사
	{
		alert_export_error("export", str);
		return ;
	}
	arr = ft_split(str, '='); //'='를 기준으로 문자열 나누기
	if (arr == NULL)
		exit_clean(mini, EXIT_FAILURE);
	key = arr[0];
	free_trash(arr); //첫번째 key부분만 제외하고 모두 메모리 해제
	value = ft_strdup(chr + 1); //매개변수로 받아온 '='의 뒷부분 문자열을 값으로 할당
	if (value == NULL) 
		exit_clean(mini, EXIT_FAILURE);
	enrol_env(mini, key, value); //환경변수에 키, 값을 등록
}

//export
int	mini_export(t_minishell *mini, int fd_out)
{
	int		i;
	char	*chr;

	g_exit = 0;
	i = 0;
	while (mini->lo->cmdline[++i].cmd && mini->lo->cmdline[i].redir_flag == 0) //export토큰부터 리다이렉션토큰 사이의 토큰만 사용
	{
		if ((key_check_char(mini->lo->cmdline[i].cmd[0])) == FAIL) //key의 형식 검사
		{
			alert_export_error("export", mini->lo->cmdline[i].cmd); 
			continue ;
		}
		chr = ft_strchr(&mini->lo->cmdline[i].cmd[1], '='); //이퀄 문자 있으면 변수에 할당
		if (chr == NULL)
			continue ;
		add_path(mini, mini->lo->cmdline[i].cmd, chr); //환경변수에 동적할당하기
	}
	if ((mini->lo->cmdline[1].cmd) == NULL) //매개변수가 없다면 기본동작 실행
		show_declare_x(mini, fd_out);
	else if ((mini->lo->cmdline[1].redir_flag) == 1) //매개변수가 리다이렉션이면 기본동작 실행
		show_declare_x(mini, fd_out);
	return (0);
}
