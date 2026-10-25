/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_substr.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/25 18:31:20 by username         #+#    #+#              */
/*   Updated: 2026/10/25 19:37:19 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	n;
	char	*res;

	if (start > ft_strlen(s))
		n = 0;
	else
		n = ft_strlen(s) - start;
	if (len < n)
		n = len;
	res = malloc(n + 1);
	if (res == NULL)
		return (NULL);
	if (n == 0)
		res[0] = '\0';
	else
		ft_strlcpy(res, s + start, n + 1);
	return (res);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char	*r;

// 	r = ft_substr("bonjour", 3, 2);
// 	printf("normal    : [%s]\n", r);
// 	free(r);
// 	r = ft_substr("bonjour", 3, 100);
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
