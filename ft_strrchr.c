/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 12:42:48 by username          #+#    #+#             */
/*   Updated: 2026/10/22 14:31:43 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	i = ft_strlen(s);
	while (i > 0)
	{
		if (s[i] == (char) c)
			return ((char *) s + i);
		i--;
	}
	if (*s == (char) c)
		return ((char *) s);
	return (0);
}
#ifdef LIBFT_MAIN_FT_STRRCHR
int	main(void)
{
	printf("ft_strrchr = %s\n", ft_strrchr("libft", 'f'));
	return (0);
}
#endif
