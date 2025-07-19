/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:58:14 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/19 21:18:13 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//#include "../../includes/minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*get_operator_str(t_token_type type)
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
