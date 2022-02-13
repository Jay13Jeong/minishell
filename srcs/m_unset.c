#include "../includes/minishell.h"

//기존 목록보다 크기가 +1된 2차원 배열을 동적할당하는 함수
void	make_new_space(t_minishell *mini, char ***temp, char ***temp2)
{
	*temp = (char **)malloc(sizeof(char *) * (mini->len)); //기존 목록 길이 - 1 + null(1) = 기존목록길이
	if (*temp == NULL)
		exit_clean(mini, EXIT_FAILURE);
	*temp2 = (char **)malloc(sizeof(char *) * (mini->len));
	if (*temp2 == NULL)
	{
		free(*temp);
		exit_clean(mini, EXIT_FAILURE);
	}
}

//지정한 키로 환경변수를 제거하는 함수
void	pop_env(t_minishell *mini, int key_index)
{
	char	**temp;
	char	**temp2;
	int		i;

	make_new_space(mini, &temp, &temp2); //새로운 목록을 담을 환경변수의 변수 동적할당
	i = -1;
	while (++i < mini->len) //새 목록으로 기존목록을 옮기기
	{
		if (i == key_index) //만약 제거할 대상이면 메모리 해제
		{
			free(mini->key[i]);
			free(mini->content[i]);
			continue ;
		}
		temp[(i - (i >= key_index))] = mini->key[i]; //해제하면 중간에 index공백이 생기므로 i를 1을 빼주면서 반복문을 돌림
		temp2[(i - (i >= key_index))] = mini->content[i];
	}
	temp[mini->len - 1] = NULL; /*구조체가 가리키는 포인터를 이사가 끝난 목록으로 변경 */
	temp2[mini->len - 1] = NULL;
	free(mini->key);
	free(mini->content);
	mini->key = temp;
	mini->content = temp2;
}

//환경변수 제거함수
void	remove_path(t_minishell *mini, char *key)
{
	int	key_index;

	key_index = find_env(mini, key); //지우려는 키가 있는지 검사
	if (key_index == -1)	//없으면 아무일도 안 일어남
		return ;
	pop_env(mini, key_index); //환경변수를 제거
	mini->len -= 1; //구조체의 환경변수 길이를 -1
}

//unset
int	mini_unset(t_minishell *mini)
{
	int			i;

	g_exit = 0;
	i = 0;
	while (mini->lo->cmdline[++i].cmd && mini->lo->cmdline[i].redir_flag == 0) //unset토큰부터 리다이렉션토큰 사이의 토큰만 사용
	{
		if ((key_check_char(mini->lo->cmdline[i].cmd[0])) == FAIL) //키형식 체크
			alert_export_error("unset", mini->lo->cmdline[i].cmd);
		else if (ft_strchr(&mini->lo->cmdline[i].cmd[1], '=') != NULL) //'='문자가 있는지 검사
			alert_export_error("unset", mini->lo->cmdline[i].cmd);
		else if ((key_check_str(mini->lo->cmdline[i].cmd, '\0')) == FAIL) //빈 문자토큰인지 검사
			alert_export_error("unset", mini->lo->cmdline[i].cmd);
		else
			remove_path(mini, mini->lo->cmdline[i].cmd); //환경변수에서 제거
	}
	return (0);
}
