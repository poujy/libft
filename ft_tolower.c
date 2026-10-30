/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_tolower.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/20 16:19:56 by username         #+#    #+#              */
/*   Updated: 2026/10/20 20:57:54 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		c += 32;
	return (c);
}
#ifdef LIBFT_MAIN_FT_TOLOWER
int	main(void)
{
	printf("ft_tolower('A') = %c\n", ft_tolower('A'));
	return (0);
}
#endif
