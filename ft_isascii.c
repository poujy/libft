/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_isascii.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/20 16:19:40 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:17:24 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c > -1 && c < 128)
	{
		return (1);
	}
	return (0);
}

// int	main(void)
// {
// 	printf("ft_isascii(127) = %d\n", ft_isascii(127));
// 	return (0);
// }
