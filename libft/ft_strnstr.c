/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:42:39 by jjeong            #+#    #+#             */
/*   Updated: 2021/05/10 21:25:26 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	idx;
	size_t	cnt;
	size_t	len_needle;

	if (*needle == '\0')
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	len_needle = ft_strlen(needle);
	idx = 0;
	while (*haystack != '\0' && idx < len)
	{
		cnt = 0;
		while (needle[cnt] != '\0' && \
				haystack[cnt] == needle[cnt] && (idx + cnt < len))
			cnt++;
		if (cnt == len_needle)
			return ((char *)haystack);
		haystack++;
		idx++;
	}
	return (NULL);
}
