/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_memcpy.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/21 08:52:16 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:26:36 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *) dest)[i] = ((const unsigned char *) src)[i];
		i++;
	}
	return (dest);
}

// int	main(void)
// {
// 	char	dst[6];

// 	ft_memcpy(dst, "libft", 6);
// 	printf("ft_memcpy = [%s]\n", dst);
// 	return (0);
// }
