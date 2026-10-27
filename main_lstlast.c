/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_lstlast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 12:12:22 by username          #+#    #+#             */
/*   Updated: 2026/10/27 12:12:22 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	main(void)
{
	t_list	*head;
	t_list	*second;
	t_list	*third;
	t_list	*last;

	head = ft_lstnew("first");
	second = ft_lstnew("second");
	third = ft_lstnew("third");
	head->next = second;
	second->next = third;
	last = ft_lstlast(head);
	if (!last)
	{
		printf("ft_lstlast returned NULL\n");
		return (1);
	}
	printf("last content = %s\n", (char *) last->content);
	if (ft_lstlast(NULL) != NULL)
	{
		printf("ft_lstlast(NULL) should return NULL\n");
		return (1);
	}
	return (0);
}
