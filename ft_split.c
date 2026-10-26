/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_split.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2022/11/13 19:51:00 by username         #+#    #+#              */
/*   Updated: 2026/10/26 09:16:39 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_freeall(char **res)
{
	size_t	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
}

static size_t	ft_numberw(char const *s, char c)
{
	size_t	i;
	size_t	res;

	i = 0;
	res = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			res++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (res);
}

static size_t	ft_numberc(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	i;

	i = 0;
	res = malloc(sizeof(*res) * (ft_numberw(s, c) + 1));
	if (!res)
		return (0);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			res[i] = ft_substr(s, 0, ft_numberc(s, c));
			s = s + ft_numberc(s, c);
			if (!res[i])
			{
				ft_freeall(res);
				return (0);
			}
			i++;
		}
	}
	res[i] = 0;
	return (res);
}
