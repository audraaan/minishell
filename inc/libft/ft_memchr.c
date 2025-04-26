/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:59:19 by nbedouan          #+#    #+#             */
/*   Updated: 2024/11/14 17:59:19 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned const char	*target;

	target = (unsigned const char *)s;
	while (n)
	{
		if (*target == (unsigned char)c)
			return ((void *)target);
		target++;
		n--;
	}
	return (NULL);
}
