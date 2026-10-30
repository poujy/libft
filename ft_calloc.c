/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_calloc.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/25 17:37:34 by username         #+#    #+#              */
/*   Updated: 2026/10/26 09:10:39 by username        ###   ########.fr        */
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
// 	int		*tab;
// 	int		i;
// 	void	*p;

// 	tab = ft_calloc(5, sizeof(int));
// 	i = 0;
// 	while (i < 5)
// 	{
// 		printf("%d ", tab[i]);
// 		i++;
// 	}
// 	printf("\n");
// 	free(tab);
// 	p = ft_calloc(0, 5);
// 	printf("nmemb=0 : %s\n", p ? "pointeur (OK)" : "NULL (KO)");
// 	free(p);
// 	p = ft_calloc(5, 0);
// 	printf("size=0  : %s\n", p ? "pointeur (OK)" : "NULL (KO)");
// 	free(p);
// 	p = ft_calloc((size_t) - 1, 2);
// 	printf("overflow: %s\n", p ? "pointeur (KO)" : "NULL (OK)");
// 	return (0);
// }
int	main(void)
{
	int *values = ft_calloc(3, sizeof(*values));
		printf("ft_calloc = [%d, %d, %d]\n", values[0], values[1], values[2]);
		free(values);
	return (0);
}
