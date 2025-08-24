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

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*get_op_str(t_token_type type)
{
	if (type == HEREDOC)
		return ("<<");
	if (type == APPEND)
		return (">>");
	if (type == REDIR_IN)
		return ("<");
	if (type == REDIR_OUT)
		return (">");
	if (type == PIPE)
		return ("|");
	return (NULL);
}

int	trickster(int *i)
{
	(*i) += 2;
	return (1);
}

int	needs_space_splitting(char *value)
{
	int	i;

	i = 0;
	if (!value)
		return (0);
	while (value[i])
	{
		if (ft_isspace(value[i]))
			return (1);
		i++;
	}
	return (0);
}
