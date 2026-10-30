/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:01:39 by username          #+#    #+#             */
/*   Updated: 2026/10/28 13:45:26 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
#ifdef LIBFT_MAIN_FT_PUTCHAR_FD
int	main(void)
{
	ft_putchar_fd('A', 1);
		ft_putchar_fd('\n', 1);
	return (0);
}
#endif
