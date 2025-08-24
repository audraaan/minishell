/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 22:02:16 by nbedouan          #+#    #+#             */
/*   Updated: 2025/04/25 22:02:22 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	cmd_builder(t_data *data)
{
	t_token	*token;
	t_cmd	*current_cmd;
	int		param_index;

	param_index = 0;
	if (!data || !data->token)
		return (*data);
	data->cmd = cmd_list(data->token);
	if (!data->cmd)
		return (*data);
	current_cmd = data->cmd;
	token = data->token;
	while (token)
		cmd_builder_bis(&token, &current_cmd, &param_index);
	if (current_cmd && current_cmd->cmd_param)
		current_cmd->cmd_param[param_index] = NULL;
	return (*data);
}

static void	cmd_builder_third(t_token **token, t_cmd **current_cmd,
								int *param_index, char *cleaned_str)
{
	if ((*current_cmd) && (*current_cmd)->cmd_param)
	{
		if ((*token)->q_type != NO_QUOTES || !(*token)->in_quote)
		{
			cleaned_str = remove_outer_quotes_cmd((*token)->str);
			if (cleaned_str)
				(*current_cmd)->cmd_param[(*param_index)] = cleaned_str;
			else
				(*current_cmd)->cmd_param[(*param_index)]
					= ft_strdup((*token)->str);
		}
		else
		{
			(*current_cmd)->cmd_param[(*param_index)]
				= ft_strdup((*token)->str);
		}
		(*param_index)++;
	}
	(*token) = (*token)->next;
}

void	cmd_builder_bis(t_token **token, t_cmd **current_cmd, int *param_index)
{
	char	*cleaned_str;

	cleaned_str = NULL;
	if (!token || !(*token) || !current_cmd || !(*current_cmd))
		return ;
	if ((*token)->type == WORD)
		cmd_builder_third(token, current_cmd, param_index, cleaned_str);
	else if (is_redirection((*token)->type))
		handle_redirection(&(*current_cmd)->file, &(*token));
	else if ((*token)->type == PIPE)
	{
		if ((*current_cmd) && (*current_cmd)->cmd_param)
			(*current_cmd)->cmd_param[(*param_index)] = NULL;
		(*current_cmd) = (*current_cmd)->next;
		(*param_index) = 0;
		(*token) = (*token)->next;
	}
}

t_cmd	*cmd_list(t_token *token)
{
	t_cmd	*head;
	t_cmd	*current;
	int		nb_pipe;
	int		i;

	i = 0;
	nb_pipe = 0;
	head = NULL;
	current = NULL;
	cmd_count(token, &nb_pipe);
	while (i <= nb_pipe)
	{
		if (cmd_list_bis(&token, &head, &current) != 0)
			return (NULL);
		i++;
	}
	return (head);
}

int	cmd_list_bis(t_token **current_token, t_cmd **head, t_cmd **current)
{
	t_cmd	*new_cmd;

	if (!current_token || !(*current_token))
		return (-1);
	new_cmd = create_new_cmd(*current_token);
	if (!new_cmd)
		return (-1);
	if (!(*head))
		(*head) = new_cmd;
	else
		(*current)->next = new_cmd;
	(*current) = new_cmd;
	while (*current_token && (*current_token)->type != PIPE)
		*current_token = (*current_token)->next;
	if (*current_token)
		*current_token = (*current_token)->next;
	return (0);
}
