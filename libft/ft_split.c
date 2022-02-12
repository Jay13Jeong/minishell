/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:41:45 by jjeong            #+#    #+#             */
/*   Updated: 2021/12/06 20:12:16 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_arr_all(char **arr, size_t cnt)
{
	while (cnt-- != 0)
		free(arr[cnt]);
}

void	word_fill(char const *s, char c, char **arr, size_t cnt)
{
	size_t	len;
	size_t	idx;

	if (*s == '\0')
		return ;
	if (*s == c)
		return (word_fill(s + 1, c, arr, cnt));
	len = 0;
	while (*(s + len) != c && *(s + len) != '\0')
		len++;
	arr[cnt] = (char *)malloc((len + 1) * sizeof(char));
	if (arr[cnt] == NULL)
		return (free_arr_all(arr, cnt));
	idx = 0;
	while (idx < len)
	{
		arr[cnt][idx] = s[idx];
		idx++;
	}
	arr[cnt][len] = '\0';
	word_fill(s + len, c, arr, cnt + 1);
}

size_t	word_count(char const *s, char c, size_t count)
{
	if (*s == '\0')
		return (count);
	if (*s == c)
		return (word_count(s + 1, c, count));
	while (*s != c && *s != '\0')
		s++;
	return (word_count(s, c, count + 1));
}

char	**ft_split(char const *s, char c)
{
	size_t	count;
	char	**arr;

	if (s == NULL)
		return (NULL);
	count = word_count(s, c, 0);
	arr = (char **)malloc((count + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	word_fill(s, c, arr, 0);
	arr[count] = NULL;
	return (arr);
}
