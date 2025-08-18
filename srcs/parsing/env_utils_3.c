/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 04:52:07 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 05:22:17 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_exit_status(t_data **data, int *i, char *str, char **res)
{
	char	*value;

	value = ft_itoa((*data)->exit_status);
	*res = join_and_free(*res, ft_strdup(value));
	free(value);
	(*i) += 2;
}

void	replace_current_token_with_list(t_data *data, t_token **current,
										t_token *new_tokens)
{
	t_token	*prev;
	t_token	*next_token;
	t_token	*last_new;
	int		was_retokenized;

	was_retokenized = (*current)->retokenized;
	prev = find_prev_token(data->token, *current);
	next_token = (*current)->next;
	free((*current)->str);
	free(*current);
	last_new = new_tokens;
	while (last_new && last_new->next)
	{
		last_new->retokenized = 1;
		last_new = last_new->next;
	}
	if (last_new)
		last_new->retokenized = 1;
	if (prev)
		prev->next = new_tokens;
	else
		data->token = new_tokens;
	if (last_new)
		last_new->next = next_token;
	*current = new_tokens;
	(*current)->retokenized = was_retokenized;
}

t_token	*find_prev_token(t_token *head, t_token *token)
{
	t_token	*current;

	if (!head || head == token)
		return (NULL);
	current = head;
	while (current && current->next)
	{
		if (current->next == token)
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	token_contains_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

//char	*remove_outer_quotes(char *str)
//{
//	int	len;
//
//	len = ft_strlen(str);
//	if (len < 2)
//		return (ft_strdup(str));
//	if ((str[0] == '"' && str[len - 1] == '"')
//		|| (str[0] == '\'' && str[len - 1] == '\''))
//	{
//		if (len == 2)
//			return (ft_strdup(""));
//		return (ft_substr(str, 1, len - 2));
//	}
//	return (ft_strdup(str));
//}
