/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 09:44:10 by username          #+#    #+#             */
/*   Updated: 2026/10/27 09:47:01 by jquer            ###   ########.fr       */
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
#ifdef LIBFT_MAIN_FT_LSTDELONE
static void	test_del(void *content)
{
	free(content);
}

int	main(void)
{
	t_list *node = ft_lstnew(ft_strdup("libft"));
		ft_lstdelone(node, test_del);
		printf("ft_lstdelone = OK\n");
	return (0);
}
#endif
