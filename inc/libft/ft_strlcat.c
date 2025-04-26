/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:57:24 by nbedouan          #+#    #+#             */
/*   Updated: 2024/11/14 17:57:24 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	slen;
	size_t	dlen;

	i = 0;
	slen = ft_strlen(src);
	dlen = ft_strlen(dest);
	if (dlen >= size)
		return (size += slen);
	while (src[i] && (dlen + i) < (size - 1))
	{
		dest[dlen + i] = src[i];
		i++;
	}
	if ((dlen + i) < size)
		dest[dlen + i] = '\0';
	return (dlen + slen);
}
