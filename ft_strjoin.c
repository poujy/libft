/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_strjoin.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/25 19:50:06 by username         #+#    #+#              */
/*   Updated: 2026/10/25 20:51:03 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	a;
	size_t	b;
	char	*res;

	a = ft_strlen(s1);
	b = ft_strlen(s2);
	res = malloc((a + b) + 1);
	if (res == NULL)
		return (NULL);
	res[0] = '\0';
	ft_strlcat(res, s1, a + b + 1);
	ft_strlcat(res, s2, a + b + 1);
	return (res);
}

#include <stdio.h>

int	main(void)
{
	char	*r;

	r = ft_strjoin("bon", "jour");
	printf("normal  : [%s]\n", r);
	free(r);
	r = ft_strjoin("", "abc");
	printf("s1 vide : [%s]\n", r);
	free(r);
	r = ft_strjoin("abc", "");
	printf("s2 vide : [%s]\n", r);
	free(r);
	r = ft_strjoin("", "");
	printf("2 vides : [%s]\n", r);
	free(r);
	return (0);
}
