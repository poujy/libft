/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_lstdelone.c                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/27 09:44:10 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:23:31 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst && del)
	{
		del(lst->content);
		free(lst);
	}
}

// static void	test_del(void *content)
// {
// 	free(content);
// }

// int	main(void)
// {
// 	t_list	*node = ft_lstnew(ft_strdup("libft"));

// 	ft_lstdelone(node, test_del);
// 	printf("ft_lstdelone = OK\n");
// 	return (0);
// }
