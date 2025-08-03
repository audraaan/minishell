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

t_token	*process_word_token(t_data *data, t_token *current, t_token *next)
{
	char	*expanded;

	if (current->q_type == 1)
		return (next);
	expanded = expand_env_var(data, current->str, &current);
	if (!expanded)
		return (next);
	free(current->str);
	current->str = expanded;
	return (next);
}

void	expand_tokens(t_data *data)
{
	t_token	*current = data->token;
	t_token	*next;

	while (current)
	{
		next = current->next;
		if (current->type == WORD && current->str)
			current = process_word_token(data, current, next);
		else
			current = next;
	}
}

//t_token	*handle_retokenization(t_data *data, t_token *current,
//									char *cleaned, t_token *next)
//{
//	t_token	*new_tokens;
//
//	new_tokens = tokenize(data, cleaned);
//	free(cleaned);
//	if (new_tokens)
//		return (replace_token_with_list(&data->token, current, new_tokens));
//	else
//	{
//		free(current->str);
//		current->str = ft_strdup("");
//		return (next);
//	}
//}
//
//t_token *handle_simple_expansion(t_token *current, char *cleaned, t_token *next)
//{
//	free(current->str);
//	current->str = cleaned;
//	return next;
//}

//t_token	*handle_simple_expansion(t_token *current, char *cleaned,
//										t_token *next)
//{
//	free(current->str);
//	current->str = cleaned;
//	return (next);
//}
//
//t_token	*process_word_token(t_data *data, t_token *current, t_token *next)
//{
//	char	*expanded;
//
//	if (current->q_type == 1)
//		return (next);
//	expanded = expand_env_var(data, current->str, &current);
//	if (!expanded)
//		return (next);
//	else if (current->q_type == 2)
//		return (handle_simple_expansion(current, expanded, next));
////	if (needs_retokenization(expanded, &current->q_type))
////		return (handle_retokenization(data, current, expanded, next));
//	else
//		return (handle_simple_expansion(current, expanded, next));
//}
//
//void	expand_tokens(t_data *data)
//{
//	t_token	*current;
//	t_token	*next;
//
//	current = data->token;
//	while (current)
//	{
//		next = current->next;
//		if (current->type == WORD && current->str)
//			current = process_word_token(data, current, next);
//		else
//			current = next;
//	}
//}
