/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeong <jjeong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:40:26 by jjeong            #+#    #+#             */
/*   Updated: 2021/12/06 20:15:13 by jjeong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;
	t_list	*node;

	if (lst == NULL)
		return (NULL);
	temp = ft_lstnew((*f)(lst->content));
	if (temp == NULL)
		return (NULL);
	node = temp;
	while (lst->next != NULL)
	{
		lst = lst->next;
		node->next = ft_lstnew((*f)(lst->content));
		if (node->next == NULL)
		{
			ft_lstclear(&temp, del);
			return (NULL);
		}
		node = node->next;
	}
	return (temp);
}
