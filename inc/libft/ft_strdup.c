/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:00:48 by nbedouan          #+#    #+#             */
/*   Updated: 2024/11/14 18:33:26 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		s_len;

	s_len = ft_strlen(s);
	dup = malloc((s_len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	dup[s_len] = 0;
	while (s_len >= 0)
	{
		dup[s_len] = s[s_len];
		s_len--;
	}
	return (dup);
}
