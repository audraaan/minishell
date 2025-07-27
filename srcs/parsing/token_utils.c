/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:58:14 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/27 21:21:01 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	er_msg_free_tok(char *arg, char *msg, t_token **token)
{
	char		*tmp;
	int			res;

	tmp = NULL;
	if (arg)
		tmp = ft_strdup(arg);
	if (token)
	{
		free_tokens(token);
		token = NULL;
	}
	res = ft_error_msg(tmp, msg);
	if (tmp && *tmp)
		free(tmp);
	return (res);
}
