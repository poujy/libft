/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 16:19:22 by username          #+#    #+#             */
/*   Updated: 2026/10/21 16:38:35 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int ch)
{
	while (*str && *str != (char) ch)
	{
		str++;
		if (*str == (char) ch)
			return ((char *) str);
	}
	if (*str == (char) ch)
		return ((char *) str);
	return (NULL);
}
int	main(void)
{
	printf("ft_strchr = %s\n", ft_strchr("libft", 'b'));
	return (0);
}
