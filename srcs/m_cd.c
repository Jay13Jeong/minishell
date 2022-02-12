#include "../includes/minishell.h"

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

void	set_path_root(t_minishell *mini)
{
	char	*temp;
	char	*path;

	temp = mini->lo->cmdline[1].cmd;
	path = ft_strjoin(getenv("HOME"), &temp[1]);
	mini->lo->cmdline[1].cmd = path;
	free(temp);
}

int	cd_to_home(t_minishell *mini, int wave_ascii)
{
	char	*home_path;
	int		index;

	if (wave_ascii == 1)
		home_path = getenv("HOME");
	else
	{
		index = find_env(mini, "HOME");
		if (index == -1)
			home_path = NULL;
		else
			home_path = mini->content[index];
	}
	if (home_path == NULL)
		alert_cd_error("", 1, 1);
	else if (chdir(home_path) == ERROR)
		alert_cd_error(home_path, 1, wave_ascii);
	return (0);
}

int	mini_cd(t_minishell *mini)
{
	g_exit = 0;
	if ((mini->lo->cmdline[1].cmd) == NULL)
		return (cd_to_home(mini, 0));
	if ((mini->lo->cmdline[1].redir_flag) == 1)
		return (cd_to_home(mini, 0));
	if (mini->lo->cmdline[1].cmd[0] == '\0')
		return (0);
	if (mini->lo->cmdline[1].cmd[0] == '~')
	{
		if (mini->lo->cmdline[1].cmd[1] == '/')
			set_path_root(mini);
		else if (mini->lo->cmdline[1].cmd[1] == '\0')
			return (cd_to_home(mini, 1));
	}
	if (chdir(mini->lo->cmdline[1].cmd) == ERROR)
		alert_cd_error(mini->lo->cmdline[1].cmd, 1, 0);
	return (0);
}
