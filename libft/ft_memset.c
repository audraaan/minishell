/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:54:29 by nbedouan          #+#    #+#             */
/*   Updated: 2024/11/14 18:31:30 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *adr, int value, size_t n)
{
	char	*str;

	str = (char *)adr;
	while (n)
	{
		*str = (char)value;
		str++;
		n--;
	}
	return (adr);
}
