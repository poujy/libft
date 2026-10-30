/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_memset.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/20 16:19:48 by username         #+#    #+#              */
/*   Updated: 2026/10/20 22:11:20 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	while (n--)
	{
		((unsigned char *) s)[n] = c;
	}
	return (s);
}
int	main(void)
{
	char buffer[6] = "abcde";
		ft_memset(buffer, 'x', 3);
		printf("ft_memset = [%s]\n", buffer);
	return (0);
}
