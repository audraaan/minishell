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

t_token	*advance_after_replacement(t_token *new_tokens)
{
	t_token	*current;

	current = new_tokens;
	while (current && current->next)
		current = current->next;
	if (current)
		current = current->next;
	else
		current = NULL;
	return (current);
}

t_token	*handle_retokenization(t_data *data, t_token *current,
									char *cleaned, t_token *next)
{
	t_token	*new_tokens;

	new_tokens = tokenize(data, cleaned);
	free(cleaned);
	if (new_tokens)
	{
		replace_token_with_list(&data->token, current, new_tokens);
		return (advance_after_replacement(new_tokens));
	}
	else
	{
		free(current->str);
		current->str = ft_strdup("");
		return (next);
	}
}

t_token	*handle_simple_expansion(t_token *current, char *cleaned,
										t_token *next)
{
	free(current->str);
	current->str = cleaned;
	return (next);
}

t_token	*process_word_token(t_data *data, t_token *current, t_token *next)
{
	char	*expanded;
	char	*cleaned;

	if (token_contains_quotes(current->str) && current->str[0])
		return (next);
	expanded = expand_env_var(data, current->str);
	if (!expanded)
		return (next);
	cleaned = remove_outer_quotes(expanded);
	free(expanded);
	if (!cleaned)
		return (next);
	if (needs_retokenization(cleaned))
		return (handle_retokenization(data, current, cleaned, next));
	else
		return (handle_simple_expansion(current, cleaned, next));
}

void	expand_tokens(t_data *data)
{
	t_token	*current;
	t_token	*next;

	current = data->token;
	while (current)
	{
		next = current->next;
		if (current->type == WORD && current->str)
			current = process_word_token(data, current, next);
		else
			current = next;
	}
}

//void expand_tokens(t_data *data)
//{
//	t_token *current = data->token;
//	t_token *next;
//	char *expanded;
//	char *cleaned;
//
//	while (current)
//	{
//		next = current->next;
//		if (current->type == WORD && current->str)
//		{
//			if (token_contains_quotes(current->str) && current->str[0])
//			{
//				current = next;
//				continue;
//			}
//			expanded = expand_env_var(data, current->str);
//			if (!expanded)
//			{
//				current = next;
//				continue;
//			}
//			cleaned = remove_quotes(expanded);
//			free(expanded);
//			if (!cleaned)
//			{
//				current = next;
//				continue;
//			}
//			if (needs_retokenization(cleaned))
//			{
//				t_token *new_tokens = tokenize(data, cleaned);
//				free(cleaned);
//				if (new_tokens)
//				{
//					replace_token_with_list(&data->token, current, new_tokens);
//					current = new_tokens;
//					while (current && current->next)
//						current = current->next;
//					if (current)
//						current = current->next;
//					else
//						current = NULL;
//				}
//				else
//				{
//					free(current->str);
//					current->str = ft_strdup("");
//					current = next;
//				}
//			}
//			else
//			{
//				free(current->str);
//				current->str = cleaned;
//				current = next;
//			}
//		}
//		else
//		{
//			current = next;
//		}
//	}
//}

//void	expand_tokens(t_data *data)
//{
//	t_token	*current;
//	char	*expanded;
////	char	*quotes_removed;
//
//	current = data->token;
//	while (current)
//	{
//		if (current->type == WORD && current->str)
//		{
//			expanded = expand_env_var(data, current->str);
//			if (!expanded)
//			{
//				current = current->next;
//				continue ;
//			}
////			quotes_removed = remove_quotes(expanded);
////			free(expanded);
////			if (!quotes_removed)
////			{
////				current = current->next;
////				continue ;
////			}
//			free(current->str);
////			current->str = quotes_removed;
//			current->str = expanded;
//		}
//		current = current->next;
//	}
//}
