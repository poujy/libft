/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 08:51:20 by username          #+#    #+#             */
/*   Updated: 2026/10/27 12:02:25 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *) malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
#ifdef LIBFT_MAIN_FT_LSTNEW
int	main(void)
{
	t_list *node = ft_lstnew("libft");
		printf("ft_lstnew = [%s]\n", (char *)node->content);
		free(node);
	return (0);
}
#endif
