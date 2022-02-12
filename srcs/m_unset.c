#include "../includes/minishell.h"

void	make_new_space(t_minishell *mini, char ***temp, char ***temp2)
{
	*temp = (char **)malloc(sizeof(char *) * (mini->len));
	if (*temp == NULL)
		exit_clean(mini, EXIT_FAILURE);
	*temp2 = (char **)malloc(sizeof(char *) * (mini->len));
	if (*temp2 == NULL)
	{
		free(*temp);
		exit_clean(mini, EXIT_FAILURE);
	}
}

void	pop_env(t_minishell *mini, int key_index)
{
	char	**temp;
	char	**temp2;
	int		i;

	make_new_space(mini, &temp, &temp2);
	temp[mini->len - 1] = NULL;
	temp2[mini->len - 1] = NULL;
	i = -1;
	while (++i < mini->len)
	{
		if (i == key_index)
		{
			free(mini->key[i]);
			free(mini->content[i]);
			continue ;
		}
		temp[(i - (i >= key_index))] = mini->key[i];
		temp2[(i - (i >= key_index))] = mini->content[i];
	}
	free(mini->key);
	free(mini->content);
	mini->key = temp;
	mini->content = temp2;
}

void	remove_path(t_minishell *mini, char *key)
{
	int	key_index;

	key_index = find_env(mini, key);
	if (key_index == -1)
		return ;
	pop_env(mini, key_index);
	mini->len -= 1;
}

int	mini_unset(t_minishell *mini)
{
	int			i;

	g_exit = 0;
	i = 0;
	while (mini->lo->cmdline[++i].cmd && mini->lo->cmdline[i].redir_flag == 0)
	{
		if ((key_check_char(mini->lo->cmdline[i].cmd[0])) == FAIL)
			alert_export_error("unset", mini->lo->cmdline[i].cmd);
		else if (ft_strchr(&mini->lo->cmdline[i].cmd[1], '=') != NULL)
			alert_export_error("unset", mini->lo->cmdline[i].cmd);
		else if ((key_check_str(mini->lo->cmdline[i].cmd, '\0')) == FAIL)
			alert_export_error("unset", mini->lo->cmdline[i].cmd);
		else
			remove_path(mini, mini->lo->cmdline[i].cmd);
	}
	return (0);
}
