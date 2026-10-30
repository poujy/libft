/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 09:38:29 by username          #+#    #+#             */
/*   Updated: 2026/10/27 09:41:33 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
}
int	main(void)
{
	t_list *list = ft_lstnew("front");
		t_list *node = ft_lstnew("back");
		ft_lstadd_back(&list, node);
		printf("ft_lstadd_back = [%s]\n", (char *)ft_lstlast(list)->content);
		free(node);
		free(list);
	return (0);
}
