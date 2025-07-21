/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:44:20 by alarroye          #+#    #+#             */
/*   Updated: 2025/02/02 19:42:22 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr(int n)
{
	if (n == -2147483648)
		ft_parsestr("-2147483648");
	else if (n < 0)
	{
		ft_putchar('-');
		ft_putnbr(n * -1);
	}
	else if (n < 10)
		ft_putchar((n + '0'));
	else
	{
		ft_putnbr(n / 10);
		ft_putchar((n % 10 + '0'));
	}
}

void	ft_putunbr(unsigned int n)
{
	if (n < 10)
		ft_putchar((n + '0'));
	else
	{
		ft_putunbr(n / 10);
		ft_putchar((n % 10 + '0'));
	}
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_parsec(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_parsestr(char *str)
{
	int	len;

	if (str == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	len = 0;
	while (str[len] != '\0')
		len++;
	write(1, str, len);
	return (len);
}
