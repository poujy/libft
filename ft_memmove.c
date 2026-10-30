/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_memmove.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/21 09:00:23 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:28:18 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (src > dest)
		return (ft_memcpy(dest, src, n));
	while (n--)
		((char *) dest)[n] = ((const char *) src)[n];
	return (dest);
}

// int	main(void)
// {
// 	char	buffer[7] = "abcdef";

// 	ft_memmove(buffer + 1, buffer, 5);
// 	printf("ft_memmove = [%s]\n", buffer);
// 	return (0);
// }
