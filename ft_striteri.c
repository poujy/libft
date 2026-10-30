/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquer <jquer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 12:46:23 by username          #+#    #+#             */
/*   Updated: 2026/10/26 12:51:35 by jquer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		(*f)(i, &s[i]);
		i++;
	}
}
static void	test_iteri(unsigned int index, char *c)
{
	(void)index;
	*c -= 32;
}

int	main(void)
{
	char text[] = "abc";
		ft_striteri(text, test_iteri);
		printf("ft_striteri = [%s]\n", text);
	return (0);
}
