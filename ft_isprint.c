/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 16:19:45 by username          #+#    #+#             */
/*   Updated: 2026/10/27 08:47:52 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c > 31 && c < 127)
		return (1);
	return (0);
}
#ifdef LIBFT_MAIN_FT_ISPRINT
int	main(void)
{
	printf("ft_isprint('A') = %d\n", ft_isprint('A'));
	return (0);
}
#endif
