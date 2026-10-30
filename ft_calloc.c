/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_calloc.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/25 17:37:34 by username         #+#    #+#              */
/*   Updated: 2026/10/30 09:16:59 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*res;
	size_t			total;
	size_t			max;

	max = (size_t) - 1;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (size > max / nmemb)
		return (NULL);
	total = nmemb * size;
	res = malloc(total);
	if (res == NULL)
		return (NULL);
	ft_bzero(res, total);
	return (res);
}

// int	main(void)
// {
// 	int	*values = ft_calloc(3, sizeof(*values));

// 	printf("ft_calloc = [%d, %d, %d]\n", values[0], values[1], values[2]);
// 	free(values);
// 	return (0);
// }
