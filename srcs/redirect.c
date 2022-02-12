#include "../includes/minishell.h"

int	check_redirect_target(t_token *token)
{
	if (token->cmd == NULL)
	{
		alert_redirect_error("", 258);
		return (FAIL);
	}
	else if (token->redir_flag == 1)
	{
		alert_redirect_error(token->cmd, 258);
		return (FAIL);
	}
	return (SUCCESS);
}

int	left_way_handler(t_minishell *mini, int i)
{
	int	oflag;
	int	test_fd;

	oflag = O_RDONLY;
	if (check_redirect_target(&mini->lo->cmdline[i + 1]) == FAIL)
		return (FAIL);
	mini->lo->redirect_filename[0] = mini->lo->cmdline[i].cmd;
	mini->lo->redirect_filename[1] = mini->lo->cmdline[i + 1].cmd;
	if (ft_strncmp("<", mini->lo->cmdline[i].cmd, 2) == 0)
	{
		test_fd = open(mini->lo->cmdline[i + 1].cmd, oflag, 0644);
		if (test_fd < 1)
		{
			alert_redirect_error(mini->lo->cmdline[i + 1].cmd, 1);
			return (FAIL);
		}
		close(test_fd);
	}
	return (SUCCESS);
}

int	right_way_handler(t_minishell *mini, int i)
{
	int	oflag;
	int	test_fd;

	oflag = O_WRONLY | O_CREAT;
	if (check_redirect_target(&mini->lo->cmdline[i + 1]) == FAIL)
		return (FAIL);
	mini->lo->redirect_filename[2] = mini->lo->cmdline[i].cmd;
	mini->lo->redirect_filename[3] = mini->lo->cmdline[i + 1].cmd;
	test_fd = -1;
	if (ft_strncmp(">", mini->lo->cmdline[i].cmd, 2) == 0)
		test_fd = open(mini->lo->cmdline[i + 1].cmd, oflag | O_TRUNC, 0644);
	else if (ft_strncmp(">>", mini->lo->cmdline[i].cmd, 3) == 0)
		test_fd = open(mini->lo->cmdline[i + 1].cmd, oflag | O_APPEND, 0644);
	if (test_fd < 1)
	{
		alert_redirect_error(mini->lo->cmdline[i + 1].cmd, 1);
		return (FAIL);
	}
	close(test_fd);
	return (SUCCESS);
}

int	way_check(t_token *tk, char c)
{
	if (c == '>')
	{
		if (ft_strncmp(">", tk->cmd, 2) == 0)
			return (SUCCESS);
		if (ft_strncmp(">>", tk->cmd, 3) == 0)
			return (SUCCESS);
	}
	if (c == '<')
	{
		if (ft_strncmp("<", tk->cmd, 2) == 0)
			return (SUCCESS);
		if (ft_strncmp("<<", tk->cmd, 3) == 0)
			return (SUCCESS);
	}
	return (FAIL);
}

int	redirect_handler(t_minishell *mini, int **pipe_fd)
{
	int	i;

	i = -1;
	if (check_flag(mini) == FAIL)
		return (ERROR);
	i = -1;
	while (++i < 4)
		mini->lo->redirect_filename[i] = NULL;
	i = -1;
	while (mini->lo->cmdline[++i].cmd != NULL)
	{
		if (mini->lo->cmdline[i].redir_flag == 0)
			continue ;
		if (way_check(&mini->lo->cmdline[i], '<') == SUCCESS)
			if (left_way_handler(mini, i) == FAIL)
				return (ERROR);
		if (way_check(&mini->lo->cmdline[i], '>') == SUCCESS)
			if (right_way_handler(mini, i) == FAIL)
				return (ERROR);
	}
	if (mini->lo->redirect_filename[0] || mini->lo->redirect_filename[2])
		return (do_redirect(mini->lo, pipe_fd));
	return (0);
}
