/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_substr.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/25 18:31:20 by username         #+#    #+#              */
/*   Updated: 2026/10/30 16:54:35 by username        ###   ########.fr        */
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
		len = 0;
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

// int	main(void)
// {
// 	char	*result = ft_substr("libft", 1, 3);

// 	printf("ft_substr = [%s]\n", result);
// 	free(result);
// 	return (0);
// }
