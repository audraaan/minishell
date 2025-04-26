/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:37:07 by alarroye          #+#    #+#             */
/*   Updated: 2025/02/02 19:42:18 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	int		res;

	if (format == NULL)
		return (-1);
	va_start(arg, format);
	res = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			res += ft_conditions(&format, arg);
		}
		else
		{
			write(1, format, 1);
			res++;
		}
		format++;
	}
	va_end(arg);
	return (res);
}

int	ft_conditions(const char **format, va_list arg)
{
	int	res;

	res = 0;
	if (**format == 'c')
		res = ft_parsec(va_arg(arg, int));
	else if (**format == 's')
		res = ft_parsestr(va_arg(arg, char *));
	else if (**format == 'p')
		res = ft_parseptr(va_arg(arg, void *));
	else if (**format == 'd' || **format == 'i')
		res = ft_parsenbr(va_arg(arg, int));
	else if (**format == 'u')
		res = ft_parseunbr(va_arg(arg, unsigned int));
	else if (**format == 'x')
		res = ft_parsexalow(va_arg(arg, unsigned int));
	else if (**format == 'X')
		res = ft_parsexaupp(va_arg(arg, unsigned int));
	else if (**format == '%')
	{
		ft_putchar('%');
		res = 1;
	}
	return (res);
}
