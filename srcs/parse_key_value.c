#include "../includes/minishell.h"

int	env_value_size(char *str, int size, t_minishell *mini)
{
	int		index;
	char	*key;

	key = ft_substr(str, 1, size);
	if (!key)
		return (0);
	index = -1;
	while (mini->key[++index])
	{
		if (!ft_strcmp(key, mini->key[index]))
		{
			free(key);
			return (ft_strlen(mini->content[index]));
		}
	}
	free(key);
	return (0);
}

int	env_key_size(char *str)
{
	int	index;

	index = 0;
	while (str[++index])
	{
		if (!(ft_isalnum(str[index]) || str[index] == '_'))
			return (index - 1);
	}
	return (index - 1);
}

int	env_cnt(char *str, int *size, t_minishell *mini)
{
	int		key_len;
	char	*status;

	key_len = 0;
	if (str[1] == '?')
	{
		status = ft_itoa(g_exit);
		*size += ft_strlen(status);
		free(status);
		return (1);
	}
	if (str[1] == '\0' || str[1] == '\"')
	{
		*size += 1;
		return (0);
	}
	key_len = env_key_size(str);
	*size += env_value_size(str, key_len, mini);
	return (key_len);
}

char	*get_content(char *key, t_minishell *mini)
{
	int	i;

	i = -1;
	while (mini->key[++i])
	{
		if (!ft_strcmp(key, mini->key[i]))
			return (mini->content[i]);
	}
	return ("");
}

int	get_key(char *src, char **key)
{
	int	key_len;

	key_len = env_key_size(src);
	*key = ft_substr(src, 1, key_len);
	return (key_len);
}
