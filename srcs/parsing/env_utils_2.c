/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:57:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:33:03 by nbedouan         ###   ########.fr       */
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

void	expand_tokens(t_data *data)
{
	t_token	*current;
	char	*expanded;
	char	*quotes_removed;

	current = data->token;
	while (current)
	{
		if (current->type == WORD && current->str)
		{
			expanded = expand_env_var(data, current->str);
			if (!expanded)
			{
				current = current->next;
				continue ;
			}
			quotes_removed = remove_quotes(expanded);
			free(expanded);
			if (!quotes_removed)
			{
				current = current->next;
				continue ;
			}
			free(current->str);
			current->str = quotes_removed;
		}
		current = current->next;
	}
}
