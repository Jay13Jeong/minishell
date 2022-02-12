#include "../includes/minishell.h"

int	single_left_redirect(t_linked_order *order)
{
	int			file_fd;
	int			oflag;

	oflag = O_RDONLY;
	file_fd = open(order->redirect_filename[1], oflag, 0644);
	if (file_fd < 1)
	{
		alert_redirect_error(order->redirect_filename[1], 1);
		return (ERROR);
	}
	dup2(file_fd, STDIN);
	close(file_fd);
	return (0);
}

int	double_left_redirect(t_linked_order *order, int **pipe_fd)
{
	char		*line;
	char		*syntax;
	int			syn_len;

	syntax = order->redirect_filename[1];
	syn_len = ft_strlen(syntax);
	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, syntax, syn_len + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, (*pipe_fd)[1]);
		free(line);
	}
	dup2((*pipe_fd)[0], STDIN);
	close((*pipe_fd)[0]);
	close((*pipe_fd)[1]);
	pipe(*pipe_fd);
	return (0);
}

int	single_right_redirect(t_linked_order *order)
{
	int			file_fd;
	int			oflag;

	oflag = O_WRONLY | O_CREAT | O_TRUNC;
	file_fd = open(order->redirect_filename[3], oflag, 0644);
	if (file_fd < 1)
	{
		alert_redirect_error(order->redirect_filename[3], 1);
		return (ERROR);
	}
	dup2(file_fd, STDOUT);
	close(file_fd);
	return (1);
}

int	double_right_redirect(t_linked_order *order)
{
	int			file_fd;
	int			oflag;

	oflag = O_WRONLY | O_CREAT | O_APPEND;
	file_fd = open(order->redirect_filename[3], oflag, 0644);
	if (file_fd < 1)
	{
		alert_redirect_error(order->redirect_filename[3], 1);
		return (ERROR);
	}
	dup2(file_fd, STDOUT);
	close(file_fd);
	return (1);
}

int	do_redirect(t_linked_order *order, int **pipe_fd)
{
	int	left_status;
	int	right_status;

	left_status = 0;
	right_status = 0;
	if (ft_strncmp("<", order->redirect_filename[0], 2) == 0)
		left_status = single_left_redirect(order);
	else if (ft_strncmp("<<", order->redirect_filename[0], 3) == 0)
		left_status = double_left_redirect(order, pipe_fd);
	if (ft_strncmp(">", order->redirect_filename[2], 2) == 0)
		right_status = single_right_redirect(order);
	else if (ft_strncmp(">>", order->redirect_filename[2], 3) == 0)
		right_status = double_right_redirect(order);
	if (left_status == -1 || right_status == -1)
		return (ERROR);
	return (right_status == 1);
}
