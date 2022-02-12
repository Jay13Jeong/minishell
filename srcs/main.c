#include "../includes/minishell.h"

int	g_exit = 0;

//동적할당된 환경변수의 키값을 모두 해제하는 함수
void	free_key_value(t_minishell *mini)
{
	int	i;

	i = 0;
	while (mini->key[i])
	{
		free(mini->key[i]);
		free(mini->content[i]);
		i++;
	}
	free(mini->key);
	free(mini->content);
}

//환경변수의 길이를 측정하는 함수
void	len_env(t_minishell *mini)
{
	int	i;

	i = 0;
	while (mini->env[i])
		i++;
	mini->len = i;
}

//메인에서 받아온 환경변수 목록 크기만큼 담을 변수를 동적할당하는 함수
void	init_struct_env(t_minishell *mini)
{
	mini->key = malloc(sizeof(char *) * (mini->len + 1));
	if (!mini->key)
		exit(EXIT_FAILURE);
	mini->content = malloc(sizeof(char *) * (mini->len + 1));
	if (!mini->content)
		exit(EXIT_FAILURE);
}

//구조체 초기화 및 시그널설정, 환경변수 동적할당후 아스키아트를 띄우는 함수
void	init_mini(t_minishell *mini, char **envp)
{
	int		i;
	char	**env_aux;

	dup2(STDIN, STDIN_BACKUP); //표준입력fd 백업
	dup2(STDOUT, STDOUT_BACKUP); //표준출력fd 백업
	init_signal(); //시그널 초기화
	ascii_art();	//아스키아트 출력
	mini->env = envp;
	len_env(mini); //환경변수 리스트 길이 대입
	init_struct_env(mini); //여기서부터는 메인에서 받아온 환경변수 리스트 동적할당하기 
	i = -1;
	while (mini->env[++i])
	{
		env_aux = ft_split(mini->env[i], '=');
		mini->key[i] = ft_strdup(env_aux[0]);
		if (env_aux[1])
			mini->content[i] = ft_strdup(env_aux[1]);
		else
			mini->content[i] = ft_strdup("");
		free_char_array(env_aux);
		env_aux = NULL;
	}
	mini->key[i] = NULL;
	mini->content[i] = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	t_minishell		mini;

	init_mini(&mini, envp); //시그널설정, 환경변수 동적할당후 아스키아트 띄우기
	while (1)
	{
		line = readline_with_path(); //터미널에 현재경로를 표시하고 입력을 받는다
		if (line != NULL)
		{
			if (line[0] != '\0' && !chk_line(line)) //빈 입력이 아니면 명령 실행
			{
				add_history(line); //방향키 위아래로 최근 입력한 명령을 불러올 수 있도록 기록
				parse(&mini, line); //Lexcial analyze을 통해 "토큰", "렉서"처리한다.
				g_exit = exec(&mini); //명령실행 후 프로세스의 반환값 업데이트
				free_all_list(mini.lo); //명령 링크드리스트 메모리 모두 해제
			}
			free(line);
			continue ;
		}
		show_exit_upline(); //exit명령시 개행을 제거하고 출력한다
		break ;
	}
	free_key_value(&mini); //동적할당된 환경변수 리스트 모두 해제
	argc = argv[0][0] + 1;
	return (0);
}
