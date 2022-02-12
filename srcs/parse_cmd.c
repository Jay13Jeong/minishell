#include "../includes/minishell.h"

static int	cal_len2(const char *s, const char c, t_gss *val)
{
	if (s[val->len] == '"' && val->close == '"')
		val->close = 0;
	else if (s[val->len] == '"' && val->close == 0)
		val->close = '"';
	else if (s[val->len] == '\'' && val->close == 0)
		val->close = '\'';
	else if (s[val->len] == '\'' && val->close == '\'')
		val->close = 0;
	if (val->begin == 0 && s[val->len] != c)
	{
		val->begin = 1;
		val->len++;
	}
	else if (val->begin == 1)
	{
		if (s[val->len] == c && val->close == 0)
			return (0);
		else if ((s[val->len] == '>' || s[val->len] == '<') && val->close == 0)
			return (0);
		else
			val->len++;
	}
	return (1);
}

static int	cal_len(const char *s, const char c)
{
	t_gss	val;

	gss_init(&val);
	if (s[val.len] == '>' || s[val.len] == '<')
	{
		while (s[val.len])
		{
			if (s[val.len] == '>' || s[val.len] == '<')
				val.len++;
			else
				return (val.len);
		}
	}
	while (s[val.len])
	{
		if (!cal_len2(s, c, &val))
			break ;
	}
	return (val.len);
}

static char	*cmd_dup(const char *line, char pivot, int *start)
{
	int		len;
	int		move;
	char	*cmd;

	len = 0;
	len = cal_len(&line[*start], pivot);
	cmd = (char *)malloc(sizeof(char) * (len + 1));
	if (!cmd)
		return (0);
	move = -1;
	while (++move < len)
		cmd[move] = line[*start + move];
	cmd[move] = '\0';
	*start += len;
	return (cmd);
}

static void	split_fail_free(t_token *res, int i)
{
	int	move;

	move = 0;
	while (move < i)
	{
		free(res[move].cmd);
		move++;
	}
	free(res);
	return ;
}

t_token	*cmd_split(const char *line, const char pivot, int move, int token_i)
{
	t_token	*res;

	if (!line)
		return (0);
	res = (t_token *)malloc(sizeof(t_token) \
		* (get_split_size(line, pivot) + 1));
	if (!res)
		return (0);
	while (line[move])
	{
		if (line[move] == pivot)
			move++;
		else
		{
			res[++token_i].cmd = cmd_dup(line, pivot, &move);
			if (!res->cmd)
			{
				split_fail_free(res, token_i);
				return (0);
			}
			res[token_i].redir_flag = 0;
		}
	}
	res[++token_i].cmd = NULL;
	return (res);
}
