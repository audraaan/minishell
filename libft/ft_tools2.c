/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:36:10 by alarroye          #+#    #+#             */
/*   Updated: 2025/02/02 19:42:28 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_parsenbr(int n)
{
	int	res;

	ft_putnbr(n);
	res = 0;
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		res++;
		n *= -1;
	}
	while (n > 9)
	{
		n /= 10;
		res++;
	}
	return (res + 1);
}

int	ft_parseunbr(unsigned int n)
{
	int	res;

	ft_putunbr(n);
	res = 0;
	while (n > 9)
	{
		n /= 10;
		res++;
	}
	return (res + 1);
}

int	ft_parsexalow(unsigned long int n)
{
	int	res;

	res = 0;
	if (n > 15)
		res += ft_parsexalow(n / 16);
	if (n % 16 < 10)
		ft_putchar((n % 16) + '0');
	else
		ft_putchar((n % 16) - 10 + 'a');
	return (res + 1);
}

int	ft_parsexaupp(unsigned long int n)
{
	int	res;

	res = 0;
	if (n > 15)
		res += ft_parsexaupp(n / 16);
	if (n % 16 < 10)
		ft_putchar((n % 16) + '0');
	else
		ft_putchar((n % 16) - 10 + 'A');
	return (res + 1);
}

int	ft_parseptr(void *ptr)
{
	if (ptr == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	write(1, "0x", 2);
	return (ft_parsexalow((unsigned long int)ptr) + 2);
}
