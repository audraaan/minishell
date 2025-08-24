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

static void	remove_outer_quotes_cmd_bis(int *i, int *j, char *str, char *result)
{
	int		current_quote_state;

	current_quote_state = 0;
	while (str[*i])
	{
		if (str[*i] == '\'' && current_quote_state != 2)
		{
			if (current_quote_state == 1)
				current_quote_state = 0;
			else
				current_quote_state = 1;
		}
		else if (str[*i] == '"' && current_quote_state != 1)
		{
			if (current_quote_state == 2)
				current_quote_state = 0;
			else
				current_quote_state = 2;
		}
		else
			result[(*j)++] = str[*i];
		(*i)++;
	}
}

char	*remove_outer_quotes_cmd(char *str)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	remove_outer_quotes_cmd_bis(&i, &j, str, result);
	result[j] = '\0';
	return (result);
}

t_cmd	*create_new_cmd(t_token *token)
{
	t_cmd	*new_cmd;
	int		param_size;
	int		i;

	i = 0;
	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	param_size = get_cmd_size(token);
	new_cmd->cmd_param = malloc(sizeof(char *) * param_size);
	if (!new_cmd->cmd_param)
	{
		free(new_cmd);
		return (NULL);
	}
	while (i < param_size)
	{
		new_cmd->cmd_param[i] = NULL;
		i++;
	}
	new_cmd->file = NULL;
	new_cmd->expanded = token->expanded;
	new_cmd->next = NULL;
	return (new_cmd);
}
