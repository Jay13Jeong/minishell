#include "../includes/minishell.h"

//동적할당 된 2차원 배열을 모두 해제하는 함수
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

//링크드리스트의 메모리를 모두 해제하는 함수
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

//문자열에 화이트 스페이스를 제외한 토큰 가능한 문자가 있는지 검사
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

//현재 링크 메모리만 해제하는 함수
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

//아스키아트를 출력하는 함수
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
		─▄▄──█░░░▀█▀░░░█──▄▄─ jjeong\n\
		█░░█─▀▄░░░░░░░▄▀─█░░█%s\n", SKY, WHITE);
	}
	else
	{
		printf("%s\
		──────▄▀▄─────▄▀▄	minishell\n\
		─────▄█░░▀▀▀▀▀░░█▄	made by\n\
		─▄▄──█░░░░░░░░░░░█──▄▄	jjeong\n\
		█▄▄█─█░░▀░░┬░░▀░░█─█▄▄█%s\n", SKY, WHITE);
	}
}
