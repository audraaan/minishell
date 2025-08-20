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
	// char	*cleaned_eof;

	*token = (*token)->next;
	if (*token && (*token)->str)
	{
		// cleaned_eof = remove_quotes((*token)->str);
		// if (cleaned_eof)
		// 	current->eof = cleaned_eof;
		// else
			current->eof = ft_strdup((*token)->str);
		*token = (*token)->next;
	}
}
