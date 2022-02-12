#include "../includes/minishell.h"

void	alert_export_error(char *cmd, char *key)
{
	ft_putstr_fd("minishell: ", STDOUT);
	ft_putstr_fd(cmd, STDOUT);
	ft_putstr_fd(": `", STDOUT);
	ft_putstr_fd(key, STDOUT);
	ft_putendl_fd("': not a valid identifier", STDOUT);
	g_exit = 1;
}

void	show_declare_x(t_minishell *mini, int fd_out)
{
	int	i;

	i = 0;
	while (i < mini->len)
	{
		ft_putstr_fd("declare -x ", fd_out);
		ft_putstr_fd(mini->key[i], fd_out);
		ft_putchar_fd('=', fd_out);
		ft_putendl_fd(mini->content[i], fd_out);
		i++;
	}
}

int	key_check_char(char c)
{
	if (ft_isalpha(c))
		return (SUCCESS);
	else if (c == '_')
		return (SUCCESS);
	return (FAIL);
}

int	key_check_str(char *str, char eof)
{
	int	i;

	i = 0;
	while (str[++i] != eof)
	{
		if (ft_isalnum(str[i]))
			continue ;
		else if (str[i] == '_')
			continue ;
		return (FAIL);
	}
	return (SUCCESS);
}

void	free_trash(char **arr)
{
	int	i;

	i = 0;
	while (arr[++i] != NULL)
		free(arr[i]);
	free(arr);
}
