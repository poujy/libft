/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 09:29:49 by username          #+#    #+#             */
/*   Updated: 2026/10/27 09:35:36 by jquer            ###   ########.fr       */
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
#ifdef LIBFT_MAIN_FT_LSTSIZE
int	main(void)
{
	t_list *list = ft_lstnew("libft");
		printf("ft_lstsize = %d\n", ft_lstsize(list));
		free(list);
	return (0);
}
#endif
