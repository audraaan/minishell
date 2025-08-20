/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 04:52:07 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/20 05:19:01 by alarroye         ###   ########lyon.fr   */
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
	int		was_expanded;

	was_expanded = (*current)->expanded;
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
	(*current)->expanded = was_expanded;
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


char	*remove_quotes(char *str)
{
	char	*result;
	int		i, j;
	int		in_single = 0;
	int		in_double = 0;

	if (!str)
		return (NULL);

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);

	i = 0;
	j = 0;

	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else
			result[j++] = str[i];
		i++;
	}

	result[j] = '\0';
	return (result);
}

// char	*remove_quotes(char *str)
// {
// 	char	*result;
// 	int		i;
// 	int		j;
// 	int		in_single;
// 	int		in_double;
// 	int		quote_count;

// 	if (!str)
// 		return (NULL);
// 	result = malloc(ft_strlen(str) + 1);
// 	if (!result)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	in_single = 0;
// 	in_double = 0;
// 	quote_count = 0;
	
// 	// Compter les guillemets pour détecter les cas complexes
// 	for (int k = 0; str[k]; k++)
// 		if (str[k] == '"' || str[k] == '\'')
// 			quote_count++;
	
// 	// Si nous avons beaucoup de guillemets, c'est probablement un cas complexe
// 	if (quote_count > 6 && ft_strchr(str, '$'))
// 	{
// 		// Pour les cas complexes, préserver plus de guillemets
// 		while (str[i])
// 		{
// 			// Préserver certains guillemets dans les cas complexes
// 			if ((str[i] == '\'' && in_double) || (str[i] == '"' && in_single))
// 				result[j++] = str[i];
// 			else if (str[i] == '\'' && !in_double)
// 			{
// 				in_single = !in_single;
// 				result[j++] = str[i];
// 			}
// 			else if (str[i] == '"' && !in_single)
// 			{
// 				in_double = !in_double;
// 				result[j++] = str[i];
// 			}
// 			else
// 				result[j++] = str[i];
// 			i++;
// 		}
// 	}
// 	else
// 	{
// 		// Comportement normal pour les cas simples
// 		while (str[i])
// 		{
// 			if (str[i] == '\'' && !in_double)
// 				in_single = !in_single;
// 			else if (str[i] == '"' && !in_single)
// 				in_double = !in_double;
// 			else
// 				result[j++] = str[i];
// 			i++;
// 		}
// 	}
// 	result[j] = '\0';
// 	return (result);
// }

// char	*remove_outer_quotes_cmd(char *str, t_quote_type q_type)
// {
// 	int	len;
// 	char *result;
// 	if (!str)
// 		return (NULL);
// 	len = ft_strlen(str);
// 	if (len < 2)
// 		return (ft_strdup(str));
// 	if (q_type == SINGLE_QUOTES && str[0] == '\'' && str[len - 1] == '\'')
// 	{
// 		if (len == 2)
// 			return (ft_strdup(""));
// 		return (ft_substr(str, 1, len - 2));
// 	}
// 	else if (q_type == DOUBLE_QUOTES && str[0] == '"' && str[len - 1] == '"')
// 	{
// 		if (len == 2)
// 			return (ft_strdup(""));
// 		result = ft_substr(str, 1, len - 2);
// 		// Préserver les variables dans les guillemets doubles
// 		return (result);
// 	}
// 	return (ft_strdup(str));
// }

// char *remove_outer_quotes_cmd(char *str, t_quote_type q_type)
// {
//     int len;
//     char *result;
//     int has_inner_quotes = 0;
//     int i;

//     if (!str)
//         return (NULL);
        
//     len = ft_strlen(str);
//     if (len < 2)
//         return (ft_strdup(str));

//     /* Check if there are quotes inside the string */
//     for (i = 1; i < len - 1; i++)
//     {
//         if (str[i] == '"' || str[i] == '\'')
//         {
//             has_inner_quotes = 1;
//             break;
//         }
//     }

//     /* Only remove outer quotes if they match the quote type and don't interfere with inner structure */
//     if (q_type == SINGLE_QUOTES && str[0] == '\'' && str[len - 1] == '\'')
//     {
//         if (len == 2)
//             return (ft_strdup(""));
//         return (ft_substr(str, 1, len - 2));
//     }
//     else if (q_type == DOUBLE_QUOTES && str[0] == '"' && str[len - 1] == '"')
//     {
//         /* Be more careful with double quotes containing complex nested structures */
//         if (has_inner_quotes && len > 10) /* Heuristic for complex cases */
//         {
//             /* For complex nested quotes, be more conservative */
//             return (ft_strdup(str));
//         }
        
//         if (len == 2)
//             return (ft_strdup(""));
//         result = ft_substr(str, 1, len - 2);
//         return (result);
//     }
    
//     return (ft_strdup(str));
// }

char *remove_outer_quotes_cmd(char *str, t_quote_type q_type)
{
	int		len;
	char	*result;
	int		i;

	if (!str)
		return (NULL);

	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if (q_type == SINGLE_QUOTES && str[0] == '\'' && str[len - 1] == '\'')
	{
		if (len == 2)
			return (ft_strdup(""));
		return (ft_substr(str, 1, len - 2));
	}
	else if (q_type == DOUBLE_QUOTES && str[0] == '"' && str[len - 1] == '"')
	{
		for (i = 1; i < len - 1; i++)
		{
			if (str[i] == '"' && (i == 1 || str[i-1] != '\\'))
			{
				return (ft_strdup(str));
			}
		}

		if (len == 2)
			return (ft_strdup(""));
		result = ft_substr(str, 1, len - 2);
		return (result);
	}

	return (ft_strdup(str));
}
