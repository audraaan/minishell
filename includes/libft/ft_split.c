/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:02:38 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/02 23:20:58 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static size_t	count_word(char const *s, char c)
//{
//	size_t	i;
//	size_t	count;

//	i = 0;
//	count = 0;
//	while (s[i])
//	{
//		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
//			count++;
//		i++;
//	}
//	return (count);
//}

// static void	free_all(char **str)
//{
//	size_t	i;

//	i = 0;
//	while (str[i])
//		free(str[i++]);
//	free(str);
//}

// static size_t	u_strlen(char const *s, char c, size_t start)
//{
//	size_t	i;

//	i = 0;
//	while (s[start] != c && s[start])
//	{
//		i++;
//		start++;
//	}
//	return (i);
//}

// static char	**fill_res(char **res, char const *s, char c)
//{
//	size_t	start;
//	size_t	i;

//	i = 0;
//	start = 0;
//	while (s[start])
//	{
//		while (s[start] == c)
//			start++;
//		if (s[start])
//		{
//			res[i] = ft_substr(s, start, u_strlen(s, c, start));
//			if (!res[i])
//				return (free_all(res), NULL);
//		}
//		while (s[start] != c && s[start] != '\0')
//			start++;
//		i++;
//	}
//	return (res);
//}

// char	**ft_split(char const *s, char c)
//{
//	char	**res;
//	size_t	word_count;

//	if (!s)
//		return (NULL);
//	word_count = count_word(s, c);
//	res = (char **)ft_calloc(word_count + 1, sizeof(char *));
//	if (!res)
//		return (NULL);
//	return (fill_res(res, s, c));
//}

// split audran
void	*ft_free(char **t, int ir)
{
	int	i;

	i = 0;
	while (i < ir)
	{
		free(t[i]);
		i++;
	}
	free(t);
	return (NULL);
}

int	cw(char const *s, char c)
{
	int	word;
	int	res;
	int	i;

	res = 0;
	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] != c && word == 0)
		{
			res++;
			word = 1;
		}
		else if (s[i] == c)
			word = 0;
		i++;
	}
	return (res);
}

char	*ft_strcdup(const char *s, char c)
{
	char	*dest;
	int		i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	dest = malloc((i + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		ir;
	int		i;

	res = malloc((cw(s, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	ir = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			res[ir++] = ft_strcdup(&s[i], c);
			if (res[ir - 1] == NULL)
				return (ft_free(res, ir - 1));
		}
		while (s[i] && s[i] != c)
			i++;
	}
	res[ir] = NULL;
	return (res);
}
