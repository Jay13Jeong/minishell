/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:36:12 by jjeong            #+#    #+#             */
/*   Updated: 2021/12/06 20:21:34 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_number(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' \
		|| c == '\r' || c == '\f' || c == '\v');
}

int	ft_atoi(char const *str)
{
	int			minus;
	long long	total;

	minus = 1;
	total = 0;
	while (ft_is_space(*str))
		str++;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			minus *= -1;
	while (ft_is_number(*str))
	{
		total *= 10;
		total += *(str++) - '0';
	}
	if (total < 0)
	{
		if (minus < 0)
			return (0);
		else
			return (-1);
	}
	return (total * minus);
}
