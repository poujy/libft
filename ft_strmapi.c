/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 10:14:24 by username          #+#    #+#             */
/*   Updated: 2026/10/26 12:37:28 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*res;
	unsigned int	i;

	i = 0;
	res = malloc(sizeof(*s) * (ft_strlen(s) + 1));
	if (!res)
		return (0);
	while (s[i])
	{
		res[i] = (*f)(i, s[i]);
		i++;
	}
	res[i] = 0;
	return (res);
}
static char	test_map(unsigned int index, char c)
{
	(void)index;
	return (c - 32);
}

int	main(void)
{
	char *result = ft_strmapi("abc", test_map);
		printf("ft_strmapi = [%s]\n", result);
		free(result);
	return (0);
}
