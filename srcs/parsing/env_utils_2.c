/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 04:52:07 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 05:22:26 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_outer_quotes(char *str, t_quote_type q_type)
{
	size_t	len;
	char	*res;
	char	target_quote;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if (q_type == DOUBLE_QUOTES)
		target_quote = '"';
	else if (q_type == SINGLE_QUOTES)
		target_quote = '\'';
	else
		return (ft_strdup(str));
	if (str[0] != target_quote || str[len - 1] != target_quote)
		return (ft_strdup(str));
	res = malloc(len - 1);
	if (!res)
		return (NULL);
	j = 0;
	i = 1;
	if (i < len - 1)
	{
		res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

//char *remove_outer_quotes(char *str)
//{
//	size_t len;
//	char	*res;
//	int		i;
//	int		j;
//	char	quote;
//
//	i = 0;
//	j = 0;
//	quote = 0;
//	len = ft_strlen(str);
//	res = malloc(len + 1);
//	if (!res)
//		return NULL;
//	while (str[i])
//	{
//		if (str[i] == '\'' || str[i] == '"')
//		{
//			if (!quote)
//				quote = str[i++];
//			else if (str[i] == quote)
//				quote = 0, i++;
//			else
//				res[j++] = str[i++];
//		}
//		else
//		{
//			res[j++] = str[i++];
//		}
//	}
//	res[j] = '\0';
//	return res;
//}

t_token	*process_word_token(t_data *data, t_token *current, t_token *next)
{
	char	*expanded;

	if (current->q_type == SINGLE_QUOTES && current->retokenized)
		return (next);
	expanded = expand_env_var(data, current->str, &current);
	if (!expanded)
		return (next);
	if (current->str)
		free(current->str);
	current->str = expanded;
	return (next);
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

static int	check_only_q(t_token **current)
{
	int	i;

	i = 0;
	if (!current || !(*current) || !(*current)->str)
		return (0);
	while ((*current)->str[i])
	{
		if ((*current)->str[i] != '"' && (*current)->str[i] != '\'')
			return (0);
		i++;
	}
	return (1);
}

void	expand_tokens(t_data *data)
{
	t_token	*current;
	t_token	*next;
	char	*clean;

	current = data->token;
	while (current)
	{
		next = current->next;
		if (current->type == WORD && current->str)
			current = process_word_token(data, current, next);
		else
			current = next;
	}
	current = data->token;
	while (current)
	{
		next = current->next;
		if (current->q_type != NO_QUOTES && current->str && !check_only_q(&current) && !current->retokenized)
		{
			clean = remove_outer_quotes(current->str, current->q_type);
			free(current->str);
			current->str = clean;
			current->q_type = NO_QUOTES;
		}
		current = next;
	}
}
