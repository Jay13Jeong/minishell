#include "../includes/minishell.h"

int	env_cpy2(char *src, char **dest, t_minishell *mini)
{
	int		src_idx;
	int		content_len;
	char	*key;
	char	*content;

	src_idx = 0;
	src_idx = get_key(src, &key);
	content = get_content(key, mini);
	content_len = ft_strlen(content);
	ft_memcpy(*dest, content, content_len);
	*dest += content_len;
	free(key);
	return (src_idx);
}

int	env_cpy(char *src, char **dest, t_minishell *mini)
{
	char	*status;

	if (src[1] == '?')
	{
		status = ft_itoa(g_exit);
		ft_memcpy(*dest, status, ft_strlen(status));
		*dest += ft_strlen(status);
		free(status);
		return (1);
	}
	if (src[1] == '\0' || src[1] == '\"')
	{
		**dest = '$';
		*dest += 1;
		return (0);
	}
	return (env_cpy2(src, dest, mini));
}

int	double_cpy(char *src, char **dest, t_minishell *mini)
{
	int	src_move;

	src_move = 1;
	while (src[src_move] && src[src_move] != '\"')
	{
		if (src[src_move] == '$')
			src_move += (env_cpy(&src[src_move], dest, mini) + 1);
		else
		{
			**dest = src[src_move];
			(*dest)++;
			src_move++;
		}
	}
	return (src_move);
}

int	single_cpy(char *src, char **dest)
{
	int	src_move;
	int	dest_move;

	src_move = 1;
	dest_move = 0;
	while (src[src_move] && src[src_move] != '\'')
	{
		src_move++;
		dest_move++;
	}
	ft_memcpy(*dest, src + 1, dest_move);
	*dest += dest_move;
	return (src_move);
}

int	rebuild_cmd(char *src, char *dest, t_minishell *mini)
{
	int		src_move;
	int		size;
	char	*dest_end;

	size = 0;
	src_move = -1;
	dest_end = dest;
	while (src[++src_move])
	{
		if (src[src_move] == '\'' || src[src_move] == '\"')
		{
			if (!quotes(mini, src, &dest_end, &src_move))
				return (0);
		}
		else if (src[src_move] == '$')
			src_move += env_cpy(&src[src_move], &dest_end, mini);
		else if (src[src_move])
		{
			*dest_end = src[src_move];
			dest_end++;
		}
	}
	*dest_end = '\0';
	return (1);
}
