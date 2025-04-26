/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:00:22 by nbedouan          #+#    #+#             */
/*   Updated: 2024/11/14 18:00:22 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	final_size;

	if (size && nmemb > SIZE_MAX / size)
		return (NULL);
	final_size = nmemb * size;
	ptr = malloc(final_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, final_size);
	return (ptr);
}
