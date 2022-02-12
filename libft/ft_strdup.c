/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:41:55 by jjeong            #+#    #+#             */
/*   Updated: 2021/05/10 01:16:53 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*arr;
	size_t	size;

	size = ft_strlen(s1);
	arr = ft_calloc(size + 1, sizeof(char));
	if (arr == NULL)
		return (NULL);
	ft_memcpy(arr, s1, size);
	arr[size] = '\0';
	return (arr);
}
