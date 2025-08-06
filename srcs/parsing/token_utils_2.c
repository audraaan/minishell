/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:58:14 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:33:22 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_double_quote(int *quotes, t_quote_type *q_type)
{
	if ((*quotes) == 2)
	{
		(*quotes) = 0;
		*q_type = NO_QUOTES;
	}
	else
	{
		(*quotes) = 2;
		if ((*q_type) == NO_QUOTES)
			(*q_type) = DOUBLE_QUOTES;
	}
}

void	handle_single_quote(int *quotes, t_quote_type *q_type)
{
	if ((*quotes) == 1 && *q_type == 1)
	{
		(*quotes) = 0;
		*q_type = NO_QUOTES;
	}
	else
	{
		(*quotes) = 1;
		if ((*q_type) == NO_QUOTES)
			(*q_type) = SINGLE_QUOTES;
	}
}

void	handle_simple_expansion(char *value, char **res)
{
	char	*tmp;

	tmp = ft_strdup(value);
	if (tmp)
		*res = join_and_free(*res, tmp);
}

void	handle_empty_var(char **res)
{
	char	*tmp;

	tmp = ft_strdup("$");
	if (tmp)
		*res = join_and_free(*res, tmp);
}
