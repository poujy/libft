/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 08:49:17 by username          #+#    #+#             */
/*   Updated: 2026/10/22 15:37:02 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *) s1)[i] != ((unsigned char *) s2)[i])
			return (((unsigned char *) s1)[i] - ((unsigned char *) s2)[i]);
		i++;
	}
	return (0);
}

// int	main(void)
// {
// 	const char	*str = "Hello";
// 	const char	*str2 = "Hello, World!";
// 	const char	*str3 = "Hi\0Test";
// 	const char	*str4 = "Hi\0Aest";

// 	printf("ma ft   : %d\n", ft_memcmp(str, str2, 3));
// 	printf("vrai ft : %d\n", memcmp(str, str2, 3));
// 	printf("ma ft   : %d\n", ft_memcmp(str, str2, 20));
// 	printf("vrai ft : %d\n", memcmp(str, str2, 20));
// 	printf("ma ft   : %d\n", ft_memcmp(str3, str4, 5));
// 	printf("vrai ft : %d\n", memcmp(str3, str4, 5));
// 	printf("ma ft   : %d\n", ft_memcmp(str3, str4, 2));
// 	printf("vrai ft : %d\n", memcmp(str4, str4, 2));
// 	return (0);
// }
int	main(void)
{
	printf("ft_memcmp = %d\n", ft_memcmp("abc", "abd", 3));
	return (0);
}
