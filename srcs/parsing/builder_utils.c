/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:57:09 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:32:45 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_count(t_token *token, int *nb_pipe)
{
	t_token	*tmp;

	if (!token)
		return ;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == PIPE)
			(*nb_pipe)++;
		tmp = tmp->next;
	}
}

int	is_redirection(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == HEREDOC || type == APPEND);
}

int	get_cmd_size(t_token *token)
{
	t_token	*tmp;
	int		nb_param;

	nb_param = 1;
	if (!token)
		return (0);
	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (is_redirection(tmp->type))
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
			continue ;
		}
		if (tmp->type == WORD)
			nb_param++;
		tmp = tmp->next;
	}
	return (nb_param);
}

t_file	*file_add_back(t_file **lst)
{
	t_file	*new;
	t_file	*current;

	new = ft_calloc(1, sizeof(t_file));
	if (!new)
		return (NULL);
	if (!*lst)
		*lst = new;
	else
	{
		current = *lst;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (new);
}

void	copy_filename(t_file *current, t_token **token)
{
	*token = (*token)->next;
	if (*token && (*token)->str)
	{
		current->filename = ft_strdup((*token)->str);
		*token = (*token)->next;
	}
}
