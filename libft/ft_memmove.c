/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:41:05 by jjeong            #+#    #+#             */
/*   Updated: 2021/05/11 02:52:07 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	void	*addr;
	int		way;

	addr = dst;
	way = 1 - (2 * (dst > src));
	if (dst == NULL && src == NULL)
		return (NULL);
	dst += (way == -1) * (len - 1);
	src += (way == -1) * (len - 1);
	while (len-- > 0)
	{
		*(unsigned char *)dst = *(unsigned char *)src;
		dst += way;
		src += way;
	}
	return (addr);
}
