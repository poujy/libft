/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_lstiter.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/27 10:04:07 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:23:41 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL || f == NULL)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

// static void	test_iter(void *content)
// {
// 	printf("[%s]", (char *) content);
// }

// int	main(void)
// {
// 	t_list	*list = ft_lstnew("libft");

// 	ft_lstiter(list, test_iter);
// 	ft_putchar_fd('\n', 1);
// 	free(list);
// 	return (0);
// }
