/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:00:48 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/04 04:26:19 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		i;

	i = 0;
	while (s[i])
		i++;
	dest = (char *)malloc(i * sizeof(char) + 1);
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
//char	*ft_strdup(const char *s)
//{
//	char	*dup;
//	int		s_len;

//	s_len = ft_strlen(s);
//	dup = malloc((s_len + 1) * sizeof(char));
//	if (!dup)
//		return (NULL);
//	dup[s_len] = 0;
//	while (s_len >= 0)
//	{
//		dup[s_len] = s[s_len];
//		s_len--;
//	}
//	return (dup);
//}
