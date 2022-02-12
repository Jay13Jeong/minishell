#include "../includes/minishell.h"

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

int	check_unique_key(t_minishell *mini, char *new_key, char *new_value)
{
	int	same_key_idx;

	same_key_idx = find_env(mini, new_key);
	if (same_key_idx != -1)
	{
		free(new_key);
		free(mini->content[same_key_idx]);
		mini->content[same_key_idx] = new_value;
		return (FAIL);
	}
	return (SUCCESS);
}
