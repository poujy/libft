/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_lstsize.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/27 09:29:49 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:25:23 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	len;

	len = 0;
	while (lst)
	{
		len++;
		lst = lst->next;
	}
	return (len);
}

// int	main(void)
// {
// 	t_list	*list = ft_lstnew("libft");

// 	printf("ft_lstsize = %d\n", ft_lstsize(list));
// 	free(list);
// 	return (0);
// }
