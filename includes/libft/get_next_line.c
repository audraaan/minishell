/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:34:12 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/28 06:54:43 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>


void	*ft_cacalloc(size_t nmemb, size_t size)
{
	void	*res;

	res = malloc(size * nmemb);
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, size * nmemb);
	return (res);
}

int	ft_n(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	else
		i = 0;
	return (i);
}

char	*ft_strrjoin(char *s1, char *s2)
{
	char	*line;
	size_t	ls1;
	size_t	ls2;
	size_t	i;

	ls1 = ft_strlen(s1);
	ls2 = 0;
	while (s2[ls2] && s2[ls2] != '\n')
		ls2++;
	if (s2[ls2] == '\n')
		ls2++;
	line = malloc((ls1 + ls2 + 1) * sizeof(char));
	if (!line)
		return (free(s1), NULL);
	i = 0;
	while (i < ls1)
	{
		line[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < ls2)
		line[ls1++] = s2[i++];
	line[ls1] = '\0';
	return (free(s1), line);
}


//char	*ft_strndup(const char *s, int n)
//{
//	char	*dest;
//	int		i;

//	i = 0;
//	while (s[i] && i < n)
//		i++;
//	dest = (char *)malloc(i * sizeof(char) + 1);
//	if (dest == NULL)
//		return (NULL);
//	i = 0;
//	while (s[i] && i < n)
//	{
//		dest[i] = s[i];
//		i++;
//	}
//	dest[i] = '\0';
//	return (dest);
//}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*dst;
	unsigned char	*s;

	s = (unsigned char *)src;
	dst = (unsigned char *)dest;
	if (dst == s)
		return (dest);
	while (n-- > 0)
		*dst++ = *s++;
	*dst++ = '\0';
	return (dest);
}

char	*ft_read(int fd, char *sspil, char *line)
{
	static char	buf[BUFFER_SIZE + 1];
	int			nb_read;
	int			len;

	ft_memset(buf, 0, BUFFER_SIZE + 1);
	while (1)
	{
		nb_read = read(fd, buf, BUFFER_SIZE);
		if (nb_read < 0)
			return (free(line), ft_memset(sspil, 0, BUFFER_SIZE + 1), NULL);
		if ((nb_read == 0) && !line[0])
			return (free(line), ft_memset(sspil, 0, BUFFER_SIZE + 1), NULL);
		buf[nb_read] = '\0';
		line = ft_strrjoin(line, buf);
		if (!line)
			return (NULL);
		len = ft_n(buf);
		if (len)
		{
			ft_memcpy(sspil, buf + len, ft_strlen(buf + len));
			return (line);
		}
		if (nb_read == 0 && line[0])
			return (line);
	}
}
char	*get_next_line(int fd)
{
	static char	sspil[BUFFER_SIZE + 1];
	char		*line;
	int			len;

	line = ft_cacalloc(sizeof(char), 1);
	if ((fd < 0) || (BUFFER_SIZE <= 0) || !line)
		return (free(line), ft_memset(sspil, 0, BUFFER_SIZE + 1), NULL);
	if (ft_n(sspil) != 0)
	{
		len = ft_n(sspil);
		line = ft_strrjoin(line, sspil);
		if (!line)
			return (NULL);
		ft_memcpy(sspil, sspil + len, ft_strlen(sspil + len));
		return (line);
	}
	else if (*sspil)
	{
		len = ft_strlen(sspil);
		line = ft_strrjoin(line, sspil);
		if (!line)
			return (NULL);
		ft_memcpy(sspil, sspil + len, ft_strlen(sspil + len));
	}
	return (ft_read(fd, sspil, line));
}


// int	main(void)
// {
// 	int		fd;
// 	int		i;
// 	char	*connard;

// 	i = 0;
// 	fd = open("./test.txt", O_RDONLY);
// 	while (i < 6)
// 	{
// 		connard = get_next_line(fd);
// 		printf("res=%s;\n", connard);
// 		free(connard);
// 		i++;
// 	}
// 	close(fd);
// 	return (0);
// }
