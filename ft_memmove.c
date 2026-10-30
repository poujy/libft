/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 09:00:23 by username          #+#    #+#             */
/*   Updated: 2026/10/27 09:25:39 by jquer            ###   ########.fr       */
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
int	main(void)
{
	char buffer[7] = "abcdef";
		ft_memmove(buffer + 1, buffer, 5);
		printf("ft_memmove = [%s]\n", buffer);
	return (0);
}
