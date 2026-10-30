/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_substr.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/25 18:31:20 by username         #+#    #+#              */
/*   Updated: 2026/10/30 08:49:33 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	n;
	char	*res;

	n = 0;
	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (NULL);
	if ((ft_strlen(s) - start) < len)
		len = ft_strlen(s) - start;
	res = malloc((len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	while (n < len)
	{
		res[n] = s[start];
		start++;
		n++;
	}
	res[n] = '\0';
	return (res);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char	*r;

// 	r = ft_substr("", 1, 1);
// 	printf("normal    : [%s]\n", r);
// 	free(r);
// 	r = ft_substr("42", 0, 2);
// 	printf("len long  : [%s]\n", r);
// 	free(r);
// 	r = ft_substr("bonjour", 10, 2);
// 	printf("start>fin : [%s]\n", r);
// 	free(r);
// 	r = ft_substr("bonjour", 7, 2);
// 	printf("start=fin : [%s]\n", r);
// 	free(r);
// 	r = ft_substr("bonjour", 0, 0);
// 	printf("len=0     : [%s]\n", r);
// 	free(r);
// 	r = ft_substr("bonjour", 0, 7);
// 	printf("tout      : [%s]\n", r);
// 	free(r);
// 	r = ft_substr("", 0, 5);
// 	printf("s vide    : [%s]\n", r);
// 	free(r);
// 	return (0);
// }
