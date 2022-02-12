/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 12:33:37 by jjeong            #+#    #+#             */
/*   Updated: 2021/05/11 12:35:37 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*s2;
	size_t	idx;
	size_t	len_s;

	if (s == NULL)
		return (NULL);
	s2 = (char *)malloc((len + 1) * sizeof(char));
	if (s2 == NULL)
		return (NULL);
	len_s = ft_strlen(s);
	idx = 0;
	while (idx < len && idx + start < len_s)
	{
		s2[idx] = s[start + idx];
		idx++;
	}
	s2[idx] = '\0';
	return (s2);
}
