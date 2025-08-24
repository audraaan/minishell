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

static int	check_str(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (1);
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
			return (1);
		i++;
	}
	return (0);
}

void	handle_double_quote_tok(int *quotes, t_quote_type *q_type,
									t_quote_type *in_quote, char *str)
{
	if ((*quotes) == 2)
	{
		(*quotes) = 0;
		if ((*q_type) == DOUBLE_QUOTES && !check_str(str))
			(*q_type) = NO_QUOTES;
	}
	else if ((*quotes) == 0)
	{
		(*quotes) = 2;
		if (*in_quote == 1)
			(*in_quote) = 3;
		else
			(*in_quote) = 2;
		if ((*q_type) == NO_QUOTES)
			(*q_type) = DOUBLE_QUOTES;
	}
}

void	handle_single_quote_tok(int *quotes, t_quote_type *q_type,
									t_quote_type *in_quote, char *str)
{
	if ((*quotes) == 1)
	{
		(*quotes) = 0;
		if ((*q_type) == SINGLE_QUOTES && !check_str(str))
			(*q_type) = NO_QUOTES;
	}
	else if ((*quotes) == 0)
	{
		(*quotes) = 1;
		if (*in_quote == 2)
			(*in_quote) = 3;
		else
			(*in_quote) = 1;
		if ((*q_type) == NO_QUOTES)
			(*q_type) = SINGLE_QUOTES;
	}
}
