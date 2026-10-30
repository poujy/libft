/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_strtrim.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/25 20:42:28 by username         #+#    #+#              */
/*   Updated: 2026/10/27 11:54:16 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	end;
	size_t	d;

	d = 0;
	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1);
	while (s1[d] != '\0' && ft_strchr(set, s1[d]) != NULL)
		d += 1;
	while (end > d && ft_strchr(set, s1[end - 1]) != NULL)
		end -= 1;
	return (ft_substr(s1, d, end - d));
}

// #include <stdio.h>

// int	main(void)
// {
// 	char	*r;

// 	r = ft_strtrim("  hello  ", " ");
// 	printf("normal    : [%s]\n", r);
// 	free(r);
// 	r = ft_strtrim("xxhixx", "x");
// 	printf("x         : [%s]\n", r);
// 	free(r);
// 	r = ft_strtrim("xhxix", "x");
// 	printf("milieu    : [%s]\n", r);
// 	free(r);
// 	r = ft_strtrim("abc", "");
// 	printf("set vide  : [%s]\n", r);
// 	free(r);
// 	r = ft_strtrim("xxxx", "x");
// 	printf("tout trim : [%s]\n", r);
// 	free(r);
// 	r = ft_strtrim("", "x");
// 	printf("s1 vide   : [%s]\n", r);
// 	free(r);
// 	r = ft_strtrim(" \thi\t ", " \t");
// 	printf("plusieurs : [%s]\n", r);
// 	free(r);
// 	return (0);
// }
#ifdef LIBFT_MAIN_FT_STRTRIM
int	main(void)
{
	char *result = ft_strtrim("--libft--", "-");
		printf("ft_strtrim = [%s]\n", result);
		free(result);
	return (0);
}
#endif
