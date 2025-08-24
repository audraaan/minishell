/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 18:24:09 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/23 18:24:13 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_word_retokenize(char *str, int *i, t_quote_type *q_type)
{
	int		start;
	int		quotes;
	char	*res;

	start = *i;
	quotes = 0;
	while (str[*i] && (!ft_isspace(str[*i]) || quotes))
	{
		if (str[*i] == '\'' && quotes != 2 && *q_type != DOUBLE_QUOTES)
			handle_single_quote(&quotes, q_type);
		else if (str[*i] == '\"' && quotes != 1 && *q_type != SINGLE_QUOTES)
			handle_double_quote(&quotes, q_type);
		(*i)++;
	}
	res = ft_substr(str, start, *i - start);
	if (!res)
		return (NULL);
	return (res);
}

static t_token	*retokenize_bis(int *i, char *str, t_quote_type *q_type,
								t_quote_type in_quote)
{
	t_token_type	type;
	t_token			*new_token;
	char			*word;

	while (str[(*i)] && ft_isspace(str[(*i)]))
		(*i)++;
	if (!str[(*i)])
		return (NULL);
	type = WORD;
	word = extract_word_retokenize(str, i, q_type);
	if (!word)
		return (NULL);
	new_token = create_token(word, type, q_type, &in_quote);
	free(word);
	if (!new_token)
		return (NULL);
	while (str[(*i)] && ft_isspace(str[(*i)]))
		(*i)++;
	return (new_token);
}

static t_token	*retokenize(t_data *data, char *str, t_quote_type o_q_type)
{
	t_token			*new_token;
	t_token			**current;
	t_quote_type	q_type;
	t_quote_type	in_quote;
	int				i;

	i = 0;
	new_token = NULL;
	in_quote = NO_QUOTES;
	current = &new_token;
	while (str[i])
	{
		q_type = o_q_type;
		*current = retokenize_bis(&i, str, &q_type, in_quote);
		if (!*current)
		{
			if (str[i])
				free_tokens(&new_token);
			break ;
		}
		current = &(*current)->next;
	}
	return (new_token);
}

static char	*handle_retoken_bis(char **res, char *value)
{
	char	*combined_value;

	combined_value = NULL;
	if (*res && **res)
		combined_value = ft_strjoin(*res, value);
	else
		combined_value = ft_strdup(value);
	return (combined_value);
}

void	handle_retoken(t_data *data, char *value, t_token **current, char **res)
{
	t_token	*new_tokens;
	char	*combined_value;
	char	*tmp;

	combined_value = handle_retoken_bis(res, value);
	if (!combined_value)
	{
		free(*res);
		*res = NULL;
		return ;
	}
	new_tokens = retokenize(data, combined_value, (*current)->q_type);
	if (new_tokens)
	{
		replace_current_token_with_list(data, current, new_tokens);
		(*current)->retokenized = 1;
	}
	else
	{
		tmp = ft_strdup(combined_value);
		if (tmp)
			*res = join_and_free(*res, tmp);
		(*current)->retokenized = 0;
	}
	free(combined_value);
}
