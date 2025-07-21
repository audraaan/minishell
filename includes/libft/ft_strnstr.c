/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:00:00 by nbedouan          #+#    #+#             */
/*   Updated: 2024/11/14 18:00:00 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *litlle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!big)
		return (NULL);
	if (!litlle[0])
		return ((char *)big);
	while (big[j] && (i + j) < len)
	{
		i = 0;
		while (litlle[i] == big[i + j] && (i + j) < len)
		{
			i++;
			if (litlle[i] == '\0')
				return ((char *)&big[j]);
		}
		j++;
	}
	return (NULL);
}
