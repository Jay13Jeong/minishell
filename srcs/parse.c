#include "../includes/minishell.h"

void	get_split_size2(const char *s, const char c, t_gss *val)
{
	if (*s == c && val->close == 0)
	{
		reset_add(val);
		val->begin = 0;
	}
	else if ((*s == '<' || *s == '>') && val->close == 0)
	{
		if (val->redir == 0 && *(s - 1) != ' ')
			val->cnt++;
		val->redir++;
	}
	else if (*s != c && *s != '\'' && *s != '\"' && !(*s == '>' || *s == '<') \
		&& val->close == 0 && (*(s - 1) == '>' || *(s - 1) == '<'))
		reset_add(val);
	else if (*s == '\'' && val->close == '\'' \
		&& (*(s - 1) == '>' || *(s - 1) == '<'))
		reset_add(val);
	else if (*s == '\"' && val->close == '\"' \
		&& (*(s - 1) == '>' || *(s - 1) == '<'))
		reset_add(val);
}

int	get_split_size(const char *s, const char c)
{
	t_gss	val;

	gss_init(&val);
	while (*s)
	{
		if (*s == '\"' && val.close == 0)
			val.close = '\"';
		else if (*s == '\"' && val.close == '\"')
			val.close = 0;
		else if (*s == '\'' && val.close == 0)
			val.close = '\'';
		else if (*s == '\'' && val.close == '\'')
			val.close = 0;
		if (val.begin == 0)
		{
			if (*s != c)
				val.begin = 1;
			if (*s == '<' || *s == '>')
				val.redir++;
		}
		else
			get_split_size2(s, c, &val);
		s++;
	}
	return (val.cnt + val.begin);
}

t_linked_order	*create_node(t_minishell *mini, char *line, \
	int val_end, int val_prev)
{
	t_linked_order	*node;

	node = (t_linked_order *)malloc(sizeof(t_linked_order));
	if (!node)
		return (0);
	node->cmdline = cmd_split(line, ' ', 0, -1);
	rebuild_token(mini, node->cmdline, -1);
	if (val_end)
		node->pipe_flag = 0;
	else
		node->pipe_flag = 1;
	if (val_prev == 0 && val_end == 1)
		node->exit_flag = 1;
	else
		node->exit_flag = 0;
	node->next = NULL;
	return (node);
}

int	insert_ls(t_minishell *mini, char *line, int val_end, int val_prev)
{
	t_linked_order	*list;
	t_linked_order	*node;

	list = mini->lo;
	node = create_node(mini, line, val_end, val_prev);
	if (!node)
		return (0);
	if (!list)
		mini->lo = node;
	else
	{
		while (list->next)
			list = list->next;
		list->next = node;
	}
	return (1);
}

void	parse(t_minishell *mini, char *line)
{
	t_parse	val;

	parse_init(&val, mini);
	while (++val.m >= 0)
	{
		if ((line[val.m] == '\'' || line[val.m] == '\"') && val.c == 0)
			val.c = 1;
		else if ((line[val.m] == '\'' || line[val.m] == '\"') && val.c == 1)
			val.c = 0;
		if ((line[val.m] == '|' && val.c == 0) || line[val.m] == '\0')
		{
			if (line[val.m] == '\0')
				val.end = 1;
			if (line[val.m] == '|')
				line[val.m] = '\0';
			if (!insert_ls(mini, &line[val.prev], val.end, val.prev))
			{
				free_all_list(mini->lo);
				return ;
			}
			if (val.end)
				break ;
			val.prev = val.m + 1;
		}
	}
}
