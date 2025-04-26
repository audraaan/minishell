/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:02:38 by nbedouan          #+#    #+#             */
/*   Updated: 2024/11/14 18:02:38 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_word(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static void	free_all(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

static size_t	u_strlen(char const *s, char c, size_t start)
{
	size_t	i;

	i = 0;
	while (s[start] != c && s[start])
	{
		i++;
		start++;
	}
	return (i);
}

static char	**fill_res(char **res, char const *s, char c)
{
	size_t	start;
	size_t	i;

	i = 0;
	start = 0;
	while (s[start])
	{
		while (s[start] == c)
			start++;
		if (s[start])
		{
			res[i] = ft_substr(s, start, u_strlen(s, c, start));
			if (!res[i])
				return (free_all(res), NULL);
		}
		while (s[start] != c && s[start] != '\0')
			start++;
		i++;
	}
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = count_word(s, c);
	res = (char **)ft_calloc(word_count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	return (fill_res(res, s, c));
}
