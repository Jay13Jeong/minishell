/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:42:51 by jjeong            #+#    #+#             */
/*   Updated: 2021/05/11 02:51:12 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	idx;
	char	*head;
	char	*foot;
	char	*dyn_alc;

	if (s1 == NULL || set == NULL)
		return (NULL);
	idx = 0;
	while (*s1 != '\0' && ft_strchr(set, *s1) != NULL)
		s1++;
	head = (char *)s1;
	if (*head == '\0')
		return (ft_strdup(""));
	while (*s1 != '\0')
		s1++;
	s1--;
	while (ft_strchr(set, *s1) != NULL)
		s1--;
	foot = (char *)s1;
	dyn_alc = (char *)malloc((foot - head + 2) * sizeof(char));
	if (dyn_alc == NULL)
		return (NULL);
	ft_memcpy(dyn_alc, head, (foot - head + 2));
	dyn_alc[foot - head + 1] = '\0';
	return (dyn_alc);
}
