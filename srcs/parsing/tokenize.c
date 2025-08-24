/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:39:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/24 02:40:47 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(char *str, int *i, t_quote_type *q_type,
		t_quote_type *in_quote)
{
	int		start;
	int		quotes;
	char	*res;

	start = *i;
	quotes = 0;
	while (str[*i] && (quotes || (!is_operator(str[*i])
				&& !ft_isspace(str[*i]))))
	{
		if (str[*i] == '\'' && quotes != 2)
			handle_single_quote_tok(&quotes, q_type, in_quote, str);
		else if (str[*i] == '\"' && quotes != 1)
			handle_double_quote_tok(&quotes, q_type, in_quote, str);
		(*i)++;
	}
	res = ft_substr(str, start, *i - start);
	if (!res)
		return (NULL);
	if (quotes)
		*in_quote = UNCLOSED;
	return (res);
}

t_token_type	get_operator_type(char *str, int *i)
{
	if (str[(*i)] == '<' && str[(*i) + 1] == '<' && trickster(i))
		return (HEREDOC);
	else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
	{
		(*i) += 2;
		return (APPEND);
	}
	else if (str[(*i)] == '>')
	{
		(*i)++;
		return (REDIR_OUT);
	}
	else if (str[(*i)] == '<')
	{
		(*i)++;
		return (REDIR_IN);
	}
	else if (str[(*i)] == '|')
	{
		(*i)++;
		return (PIPE);
	}
	return (WORD);
}

t_token	*create_token(char *str, t_token_type type, t_quote_type *q_type,
		t_quote_type *in_quote)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	if (!new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->retokenized = 0;
	new_token->expanded = 0;
	new_token->q_type = *q_type;
	new_token->in_quote = *in_quote;
	new_token->next = NULL;
	return (new_token);
}
t_token	*tokenize_bis(int *i, char *str)
{
	t_token_type	type;
	t_token			*new_token;
	char			*word;
	t_quote_type	in_quote;
	t_quote_type	q_type;

	in_quote = NO_QUOTES;
	q_type = NO_QUOTES;
	while (str[(*i)] && ft_isspace(str[(*i)]))
		(*i)++;
	if (!str[(*i)])
		return (NULL);
	if (is_operator(str[(*i)]))
	{
		type = get_operator_type(str, i);
		new_token = create_token(get_op_str(type), type, &q_type, &in_quote);
	}
	else
	{
		type = WORD;
		word = extract_word(str, i, &q_type, &in_quote);
		if (!word)
			return (NULL);
		new_token = create_token(word, type, &q_type, &in_quote);
		free(word);
		if (!new_token)
			return (NULL);
	}
	while (str[(*i)] && ft_isspace(str[(*i)]))
		(*i)++;
	return (new_token);
}

t_token	*tokenize(t_data *data, char *str)
{
	t_token	**current;
	t_token	*new_token;
	int		i;

	i = 0;
	new_token = NULL;
	current = &new_token;
	while (str[i])
	{
		*current = tokenize_bis(&i, str);
		if (!*current)
		{
			free_tokens(&new_token);
			break ;
		}
		current = &(*current)->next;
	}
	return (new_token);
}
