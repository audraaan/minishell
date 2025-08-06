/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:43:57 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/04 03:44:04 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_return(char *s1, char *s2)
{
	if (s1)
		free(s1);
	if (s2)
		free(s1);
	return (NULL);
}

int	er_msg_free_tok(char *arg, char *msg, t_token **token)
{
	char	*tmp;
	int		res;

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
