/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 00:24:12 by jjeong            #+#    #+#             */
/*   Updated: 2021/12/06 20:16:43 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	get_int_len(int n)
{
	int	len;

	len = 1 + (n < 0);
	while ((n / 10) != 0)
	{
		n /= 10;
		len += 1;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*arr;
	int		len;
	long	n2;

	n2 = n;
	len = get_int_len(n);
	arr = (char *)malloc((len + 1) * sizeof(char));
	if (arr == NULL)
		return (NULL);
	arr[0] = '0';
	if (n2 < 0)
	{
		n2 *= -1;
		arr[0] = '-';
		arr[1] = '0';
	}
	arr[len] = '\0';
	while (n2 != 0)
	{
		len -= 1;
		arr[len] = (n2 % 10) + '0';
		n2 /= 10;
	}
	return (arr);
}
