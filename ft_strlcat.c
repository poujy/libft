/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 10:48:47 by username          #+#    #+#             */
/*   Updated: 2026/10/27 08:48:15 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t destsize)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;

	dest_len = 0;
	while (dest_len < destsize && dest[dest_len])
		dest_len++;
	src_len = 0;
	while (src[src_len])
		src_len++;
	if (destsize == 0 || destsize <= dest_len)
		return (destsize + src_len);
	i = 0;
	while (src[i] && dest_len + i < destsize - 1)
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest_len + src_len);
}
#ifdef LIBFT_MAIN_FT_STRLCAT
int	main(void)
{
	char dst[10] = "lib";
		printf("ft_strlcat = %zu\n", ft_strlcat(dst, "ft", sizeof(dst)));
	return (0);
}
#endif
