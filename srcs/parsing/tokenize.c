/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:39:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/04/14 14:39:41 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(char *str, int *i, t_quote_type *q_type)
{
	int		start;
	int		quotes;
	char	*res;

	start = *i;
	quotes = 0;
	while (str[*i] && (quotes || (!is_operator(str[*i])
				&& !ft_isspace(str[*i]))))
	{
		if (str[*i] == '\'' && quotes != 2 && *q_type != 2)
			handle_single_quote(&quotes, q_type);
		else if (str[*i] == '\"' && quotes != 1 && *q_type != 1)
			handle_double_quote(&quotes, q_type);
		(*i)++;
	}
	res = ft_substr(str, start, *i - start);
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

t_token	*create_token(char *str, t_token_type type, t_quote_type *q_type)
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
	new_token->q_type = *q_type;
	new_token->next = NULL;
	return (new_token);
}

t_token	*tokenize_bis(int *i, char *str, t_quote_type *q_type)
{
	t_token_type	type;
	t_token			*new_token;
	char			*word;

	while (str[(*i)] && ft_isspace(str[(*i)]))
		(*i)++;
	if (!str[(*i)])
		return (NULL);
	if (is_operator(str[(*i)]))
	{
		type = get_operator_type(str, i);
		new_token = create_token(get_operator_str(type), type, q_type);
	}
	else
	{
		type = WORD;
		word = extract_word(str, i, q_type);
		new_token = create_token(word, type, q_type);
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
	t_token			**current;
	t_token			*new_token;
	t_quote_type	q_type;
	int				i;

	i = 0;
	q_type = NO_QUOTES;
	new_token = NULL;
	current = &new_token;
	while (str[i])
	{
		*current = tokenize_bis(&i, str, &q_type);
		if (!*current)
		{
			free_tokens(&new_token);
			break ;
		}
		current = &(*current)->next;
	}
	return (new_token);
}
