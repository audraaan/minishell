/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:43:05 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/29 18:26:15 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 20
# endif

# include <stdarg.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	char			*content;
	char			*name;
	struct s_list	*next;
}					t_list;

int					ft_toupper(int c);
int					ft_tolower(int c);
int					ft_isprint(int c);
int					ft_isdigit(int c);
int					ft_isascii(int c);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
int					ft_lstsize(t_list *lst);
int					ft_atoi(const char *str);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(char *s1, char *s2);
size_t				ft_strlen(const char *str);
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
size_t				ft_strlcat(char *dest, const char *src, size_t size);
char				*ft_itoa(int n);
char				*ft_strdup(const char *s);
char				*ft_strndup(const char *s, int n);
char				*ft_strchr(const char *s, int c);
char				**ft_split(char const *s, char c);
char				*ft_strrchr(const char *s, int c);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strnstr(const char *whole, const char *part,
						size_t len);
void				ft_bzero(void *s, size_t n);
void				ft_putnbr_fd(int n, int fd);
void				ft_putchar_fd(char c, int fd);
int					ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				*ft_calloc(size_t nmemb, size_t size);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memset(void *adr, int value, size_t n);
void				ft_lstadd_front(t_list **lst, t_list *new);
void				ft_lstiter(t_list *lst, void (*f)(void *));
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
t_list				*ft_lstlast(t_list *lst);
t_list				*ft_lstnew(char *content, char *name);
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
int					ft_printf(const char *format, ...);
int					ft_conditions(const char **format, va_list arg);
int					ft_parsec(char c);
int					ft_parsestr(char *str);
int					ft_parsenbr(int n);
int					ft_parseunbr(unsigned int n);
int					ft_parsexalow(unsigned long int n);
int					ft_parsexaupp(unsigned long int n);
int					ft_parseptr(void *ptr);
void				ft_putchar(char c);
void				ft_putnbr(int n);
void				ft_putunbr(unsigned int n);

char				*get_next_line(int fd);

#endif