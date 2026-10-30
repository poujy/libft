/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_memchr.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/21 17:47:04 by username         #+#    #+#              */
/*   Updated: 2026/10/25 17:48:14 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int c, size_t count)

{
	const unsigned char	*str;

	str = (const unsigned char *) ptr;
	while (count--)
	{
		if (*str == (unsigned char) c)
			return ((void *) str);
		str++;
	}
	return (NULL);
}
#ifdef LIBFT_MAIN_FT_MEMCHR
int	main(void)
{
	printf("ft_memchr = %s\n", (char *)ft_memchr("libft", 'f', 5));
	return (0);
}
#endif
