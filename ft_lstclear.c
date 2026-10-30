/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_lstclear.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/27 09:57:25 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:23:11 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*next;

	if (lst == NULL || del == NULL)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		del(current->content);
		free(current);
		current = next;
	}
	*lst = NULL;
}

// static void	test_del(void *content)
// {
// 	free(content);
// }

// int	main(void)
// {
// 	t_list	*list = ft_lstnew(ft_strdup("libft"));

// 	ft_lstclear(&list, test_del);
// 	printf("ft_lstclear = %s\n", list == NULL ? "OK" : "KO");
// 	return (0);
// }
