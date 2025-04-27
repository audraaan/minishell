/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:40:12 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/27 11:49:26 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, int n)
{
	char	*dup;
	int		i;

	if (n <= 0)
		return (NULL);
	if (n > ft_strlen(s))
		n = ft_strlen(s);
	dup = malloc((n + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	dup[n] = '\0';
	i = -1;
	while (++i < n)
	{
		dup[i] = s[i];
	}
	return (dup);
}
