#include "../includes/minishell.h"

int	quotes(t_minishell *mini, char *src, char **dest, int *src_move)
{
	if (src[*src_move] == '\'')
	{
		if (unclosed(&src[*src_move], '\''))
			*src_move += single_cpy(&src[*src_move], dest);
		else
		{
			mini->error = 1;
			return (0);
		}
	}
	else
	{
		if (unclosed(&src[*src_move], '\"'))
			*src_move += double_cpy(&src[*src_move], dest, mini);
		else
		{
			mini->error = 1;
			return (0);
		}
	}
	return (1);
}

void	destroy_token(t_token *token)
{
	int	i;

	i = 0;
	while (token[i].cmd)
	{
		free(token[i].cmd);
		token[i].cmd = NULL;
		i = 0;
	}
}
