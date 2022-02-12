#include "../includes/minishell.h"

int	g_exit = 0;

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

void	len_env(t_minishell *mini)
{
	int	i;

	i = 0;
	while (mini->env[i])
		i++;
	mini->len = i;
}

void	init_struct_env(t_minishell *mini)
{
	mini->key = malloc(sizeof(char *) * (mini->len + 1));
	if (!mini->key)
		exit(EXIT_FAILURE);
	mini->content = malloc(sizeof(char *) * (mini->len + 1));
	if (!mini->content)
		exit(EXIT_FAILURE);
}

void	init_mini(t_minishell *mini, char **envp)
{
	int		i;
	char	**env_aux;

	dup2(STDIN, STDIN_BACKUP);
	dup2(STDOUT, STDOUT_BACKUP);
	init_signal();
	ascii_art();
	mini->env = envp;
	len_env(mini);
	init_struct_env(mini);
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

	init_mini(&mini, envp);
	while (1)
	{
		line = readline_with_path();
		if (line != NULL)
		{
			if (line[0] != '\0' && !chk_line(line))
			{
				add_history(line);
				parse(&mini, line);
				g_exit = exec(&mini);
				free_all_list(mini.lo);
			}
			free(line);
			continue ;
		}
		show_exit_upline();
		break ;
	}
	free_key_value(&mini);
	argc = argv[0][0] + 1;
	return (0);
}
