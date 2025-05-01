/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 22:02:16 by nbedouan          #+#    #+#             */
/*   Updated: 2025/05/01 12:34:06 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_count(t_token *tmp, int *nb_pipe)
{
	while (tmp)
	{
		if (tmp->type == PIPE)
			(*nb_pipe)++;
		tmp = tmp->next;
	}
}

void	param_count(t_token *tmp, int *nb_param)
{
	while (tmp)
	{
		if (tmp->type != PIPE)
			(*nb_param)++;
		tmp = tmp->next;
	}
}

t_cmd	*cmd_list(t_token *token)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*tmp;
	int		nb_pipe;
	int		nb_token;
	int		i;
	t_cmd	*new_cmd;

	i = 0;
	nb_pipe = 0;
	nb_token = 0;
	head = NULL;
	current = NULL;
	tmp = token;
	cmd_count(tmp, &nb_pipe);
	param_count(tmp, &nb_token);
	while (i <= nb_pipe)
	{
		new_cmd = malloc(sizeof(t_cmd));
		if (!new_cmd)
			return (NULL);
		new_cmd->cmd_param = ft_calloc(sizeof(char *), (nb_pipe + 1));
			// nbr de cmd param
		//new_cmd->fd_in = NULL;
		//new_cmd->fd_out = NULL;
		new_cmd->next = NULL;
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		i++;
	}
	return (head);
}

t_data	cmd_builder(t_data *data)
{
	int i;
	t_token *token;
	t_cmd *cmd;

	i = 0;
	token = NULL;
	if (!data->token)
		return (*data);
	data->cmd = cmd_list(token);
	if (!data->cmd)
		return (*data);
	token = data->token;
	cmd = data->cmd;
	while (token)
	{
		if (token->type != PIPE)
		{
			cmd->cmd_param[i] = ft_strdup(token->str);
			token = token->next;
			i++;
		}
		else
		{
			cmd->cmd_param[i] = NULL;
			cmd = cmd->next;
			token = token->next;
			i = 0;
		}
	}
	if (cmd)
		cmd->cmd_param[i] = NULL;
	return (*data);
}