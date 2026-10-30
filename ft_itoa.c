/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 09:17:24 by username          #+#    #+#             */
/*   Updated: 2026/10/26 11:29:22 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_nbr(long nb)
{
	size_t	count;

	count = 0;
	if (nb <= 0)
		count++;
	while (nb != 0)
	{
		nb /= 10;
		count++;
	}
	return (count);
}

static char	*fill_str(char *str, long nb, size_t len)
{
	str[len] = '\0';
	if (nb == 0)
		str[0] = '0';
	while (nb != 0)
	{
		len--;
		str[len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	long	nb;
	size_t	len;
	char	*str;

	nb = n;
	len = count_nbr(nb);
	str = (char *) malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	return (fill_str(str, nb, len));
}
#ifdef LIBFT_MAIN_FT_ITOA
int	main(void)
{
	char *result = ft_itoa(-42);
		printf("ft_itoa = [%s]\n", result);
		free(result);
	return (0);
}
#endif
