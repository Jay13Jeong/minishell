#include "../includes/minishell.h"

void	free_char_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

void	free_all_list(t_linked_order *order)
{
	t_linked_order		*tmp;
	t_linked_order		*next;
	int					i;

	tmp = order;
	while (tmp)
	{
		next = tmp->next;
		i = 0;
		while (tmp->cmdline[i].cmd != NULL)
			free(tmp->cmdline[i++].cmd);
		free(tmp->cmdline);
		free(tmp);
		tmp = next;
	}
}

int	chk_line(char *line)
{
	int		i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != 32 && !(line[i] >= 9 && line[i] <= 13))
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}

void	free_linked_order(t_minishell *mini)
{
	t_linked_order		*next;
	int					i;

	if (mini->lo == NULL)
		return ;
	next = mini->lo->next;
	i = 0;
	while (mini->lo->cmdline[i].cmd != NULL)
		free(mini->lo->cmdline[i++].cmd);
	free(mini->lo->cmdline);
	free(mini->lo);
	mini->lo = next;
}

void	ascii_art(void)
{
	int	random;

	random = (int)(long long)&random;
	if (random % 3)
	{
		printf("%s\
		───▄▀▀▀▄▄▄▄▄▄▄▀▀▀▄───\n\
		───█▒▒░░░░░░░░░▒▒█─── minishell\n\
		────█░░█░░░░░█░░█──── made by\n\
		─▄▄──█░░░▀█▀░░░█──▄▄─ jjeong seungyki\n\
		█░░█─▀▄░░░░░░░▄▀─█░░█%s\n", SKY, WHITE);
	}
	else
	{
		printf("%s\
		──────▄▀▄─────▄▀▄	minishell\n\
		─────▄█░░▀▀▀▀▀░░█▄	made by\n\
		─▄▄──█░░░░░░░░░░░█──▄▄	jjeong seungyki\n\
		█▄▄█─█░░▀░░┬░░▀░░█─█▄▄█%s\n", SKY, WHITE);
	}
}
