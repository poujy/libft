/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_lstmap.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/27 10:06:30 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:25:12 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	void	*tmp;

	if (!lst || !f)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		tmp = f(lst->content);
		new_node = ft_lstnew(tmp);
		if (new_node == NULL)
		{
			del(tmp);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}

// static void	*test_map_list(void *content)
// {
// 	return (ft_strdup((char *) content));
// }

// static void	test_del(void *content)
// {
// 	free(content);
// }

// int	main(void)
// {
// 	t_list	*list = ft_lstnew("libft");
// 	t_list	*mapped = ft_lstmap(list, test_map_list, test_del);

// 	printf("ft_lstmap = [%s]\n", (char *) mapped->content);
// 	free(mapped->content);
// 	free(mapped);
// 	free(list);
// 	return (0);
// }
