/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:36:24 by jjeong            #+#    #+#             */
/*   Updated: 2021/05/06 03:36:24 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t len, size_t size)
{
	void	*arr;
	size_t	total;

	total = len * size;
	arr = malloc(total);
	if (arr == NULL)
		return (NULL);
	ft_bzero(arr, total);
	return (arr);
}
