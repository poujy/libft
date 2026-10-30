/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 17:09:35 by username          #+#    #+#             */
/*   Updated: 2026/10/30 12:42:40 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n && *s1 && (*s1 == *s2))
	{
		++s1;
		++s2;
		--n;
	}
	if (n == 0)
		return (0);
	return (*(unsigned char *) s1 - *(unsigned char *) s2);
}

// int	main(void)
// {
// 	printf("ft_strncmp = %d\n", ft_strncmp("abc", "abd", 3));
// 	return (0);
// }
