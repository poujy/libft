/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 09:18:27 by username          #+#    #+#             */
/*   Updated: 2026/10/27 09:20:25 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}
int	main(void)
{
	t_list *list = ft_lstnew("back");
		t_list *node = ft_lstnew("front");
		ft_lstadd_front(&list, node);
		printf("ft_lstadd_front = [%s]\n", (char *)list->content);
		free(node);
		free(list);
	return (0);
}
