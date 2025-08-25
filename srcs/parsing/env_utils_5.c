/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:54:08 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/24 02:54:21 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_var_name(char *str, int *i)
{
	char	*name;
	int		start;

	(*i)++;
	start = (*i);
	while (str[(*i)] && (ft_isalnum(str[(*i)]) || str[(*i)] == '_'))
		(*i)++;
	name = ft_substr(str, start, (*i) - start);
	return (name);
}

int	check_token(t_token **current)
{
	int	i;

	i = 0;
	if (!current || !(*current) || !(*current)->str)
		return (1);
	while ((*current)->str[i])
	{
		if ((*current)->str[i] != '"' && (*current)->str[i] != '\'')
			return (1);
		i++;
	}
	return (0);
}

int	check_q(t_token **current)
{
	int	i;

	i = 0;
	if (!(*current) || !(*current)->str)
		return (0);
	while ((*current)->str[i])
	{
		if ((*current)->str[i] != '"' && (*current)->str[i] != '\'')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_loop_remove_outer_quotes(int len, char *str)
{
	int	i;

	i = 1;
	while (i < len - 1)
	{
		if ((str[i] == '"' || str[i] == '\'') && i == 1)
			return (1);
		i++;
	}
	return (0);
}

char	*remove_outer_quotes(char *str, t_quote_type q_type)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((q_type == SINGLE_QUOTES && str[0] == '\'' && str[len - 1] == '\'')
		|| (q_type == DOUBLE_QUOTES && str[0] == '"' && str[len - 1] == '"'))
	{
		if (ft_loop_remove_outer_quotes(len, str))
			return (ft_strdup(str));
		if (len == 2)
			result = ft_strdup("");
		else
			result = ft_substr(str, 1, len - 2);
	}
	else
		result = ft_strdup(str);
	if (!result)
		return (NULL);
	return (result);
}
