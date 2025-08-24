/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 06:37:36 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/17 06:37:51 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_file **files, t_token **token)
{
	t_file	*current;

	if (!files || !token || !*token)
		return ;
	current = file_add_back(files);
	if (!current)
		return ;
	current->type = (*token)->type;
	if (current->type == REDIR_IN
		|| current->type == REDIR_OUT
		|| current->type == APPEND)
	{
		copy_filename(current, token);
	}
	else if (current->type == HEREDOC)
		copy_eof(current, token);
}

void	copy_eof(t_file *current, t_token **token)
{
	*token = (*token)->next;
	if (*token && (*token)->str)
	{
		current->eof = ft_strdup((*token)->str);
		*token = (*token)->next;
	}
}

char	*remove_outer_quotes_cmd(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		current_quote_state;

	i = 0;
	j = 0;
	current_quote_state = 0;
	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && current_quote_state != 2)
		{
			if (current_quote_state == 1)
				current_quote_state = 0;
			else
				current_quote_state = 1;
		}
		else if (str[i] == '"' && current_quote_state != 1)
		{
			if (current_quote_state == 2)
				current_quote_state = 0;
			else
				current_quote_state = 2;
		}
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
