#include "../includes/minishell.h"

//cd함수 예러 처리 함수
void	alert_cd_error(char *str, int exit_code, int wave_ascii)
{
	g_exit = exit_code;
	if (wave_ascii == 1)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDOUT);
		return ;
	}
	ft_putstr_fd("minishell: cd: ", STDOUT);
	ft_putstr_fd(str, STDOUT);
	ft_putendl_fd(": No such file or directory", STDOUT);
}

//동적할당된 환경변수에서 인자로 받은 key를 찾는 함수
int	find_env(t_minishell *mini, char *key)
{
	int	i;
	int	len_key;

	i = -1;
	len_key = ft_strlen(key);
	while (mini->key[++i])
		if (ft_strncmp(mini->key[i], key, len_key + 1) == 0)
			return (i);
	return (-1);
}

//이동경로의 '~'아스키를 homepath로 재설정하는 함수 
int	set_path_wave(t_minishell *mini)
{
	char	*temp;
	char	*path;

	temp = mini->lo->cmdline[1].cmd;
	path = ft_strjoin(getenv("HOME"), &temp[1]); //'~'를 home path로 변경해서 재할당
	free(temp);
	mini->lo->cmdline[1].cmd = path;
	if (chdir(mini->lo->cmdline[1].cmd) == ERROR) //경로이동에 실패하면 오류처리
		alert_cd_error(mini->lo->cmdline[1].cmd, 1, 0);
	return (0);
}

//홈으로 이동하는 함수
int	cd_to_home(t_minishell *mini, int wave_ascii)
{
	char	*home_path;
	int		index;

	if (wave_ascii == 1) //'~'면 메인에서 받아왔던 환경변수에서 경로를 찾는다.
		home_path = getenv("HOME");
	else				//'~'가 아니면 동적할당된 환경변수에서 경로를 찾는다.
	{
		index = find_env(mini, "HOME");
		if (index == -1)
			home_path = NULL;
		else
			home_path = mini->content[index];
	}
	if (home_path == NULL) //홈 경로가 없다면 오류처리
		alert_cd_error("", 1, 1);
	else if (chdir(home_path) == ERROR) //이동에 실패하면 에러처리
		alert_cd_error(home_path, 1, wave_ascii);
	return (0);
}

//cd
int	mini_cd(t_minishell *mini)
{
	g_exit = 0;
	if ((mini->lo->cmdline[1].cmd) == NULL) //이동할 경로가 null이면 홈으로간다
		return (cd_to_home(mini, 0));
	if ((mini->lo->cmdline[1].redir_flag) == 1) //이동할 경로가 리다이렉션이면 홈으로
		return (cd_to_home(mini, 0));
	if (mini->lo->cmdline[1].cmd[0] == '\0') //이동할 경로가 ''면 가만히 있는다
		return (0);
	if (mini->lo->cmdline[1].cmd[0] == '~') //'~'면 환경변수가 지워져도 홈으로 이동한다.
	{
		if (mini->lo->cmdline[1].cmd[1] == '/') //슬래쉬가 이어서 붙으면 그 뒤 경로들도 연결한다.
			return (set_path_wave(mini));
		else if (mini->lo->cmdline[1].cmd[1] == '\0') //'~'만 있으면 그냥 홈으로
			return (cd_to_home(mini, 1));
	}
	if (chdir(mini->lo->cmdline[1].cmd) == ERROR) //경로이동에 실패하면 오류처리
		alert_cd_error(mini->lo->cmdline[1].cmd, 1, 0);
	return (0);
}
