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

void	cmd_builder_bis(t_token **token, t_cmd **current_cmd, int *param_index)
{
	if (!token || !(*token) || !current_cmd || !(*current_cmd))
		return ;
	if ((*token)->type == WORD)
	{
		if ((*current_cmd) && (*current_cmd)->cmd_param)
		{
			(*current_cmd)->cmd_param[(*param_index)]
				= ft_strdup((*token)->str);
			(*param_index)++;
		}
		(*token) = (*token)->next;
	}
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
	t_token	*current_token;
	int		nb_pipe;
	int		i;

	i = 0;
	nb_pipe = 0;
	head = NULL;
	current = NULL;
	current_token = token;
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
	new_cmd->next = NULL;
	return (new_cmd);
}

//t_cmd	*cmd_list(t_token *token)
//{
//	t_cmd		*head;
//	t_cmd		*current;
//	t_cmd		*new_cmd;
//	t_token		*current_token;
//	int			nb_pipe;
//	int			i;
//
//	i = 0;
//	nb_pipe = 0;
//	head = NULL;
//	current = NULL;
//	current_token = token;
//	cmd_count(token, &nb_pipe);
//	while (i <= nb_pipe)
//	{
//		new_cmd = create_new_cmd(current_token);
//		if (!new_cmd)
//			return (NULL);
//		if (!head)
//			head = new_cmd;
//		else
//			current->next = new_cmd;
//		current = new_cmd;
//		i++;
//		while (current_token && current_token->type != PIPE)
//			current_token = current_token->next;
//		if (current_token)
//			current_token = current_token->next;
//	}
//	return (head);
//}

//void	handle_redirection(t_cmd *current_cmd, t_token *token)
//{
//	if (token->type == REDIR_IN || token->type == HEREDOC)
//	{
//		token = token->next;
//		if (token && current_cmd)
//		{
//			free(current_cmd->file_in);
//			current_cmd->file_in = ft_strdup(token->str);
//		}
//	}
//	else if (token->type == REDIR_OUT || token->type == APPEND)
//	{
//		token = token->next;
//		if (token && current_cmd)
//		{
//			free(current_cmd->file_out);
//			current_cmd->file_out = ft_strdup(token->str);
//		}
//	}
//	if (token)
//		token = token->next;
//}

//void handle_redirection(t_cmd *cmd, t_token **token)
//{
//	if (!token || !*token)
//		return;
//
//	cmd->file->type = (*token)->type;
//	if ((*token)->type == REDIR_IN)
//	{
//		*token = (*token)->next;
//		if (*token && (*token)->str)
//		{
//			cmd->file.file = ft_strdup((*token)->str);
//			cmd->file = file->next;
//			*token = (*token)->next;
//		}
//	}
//	else if ((*token)->type == HEREDOC)
//	{
//		*token = (*token)->next;
//		if (*token && (*token)->str)
//		{
//			file.file = ft_strdup((*token)->str);
//			file = file.next;
//			*token = (*token)->next;
//		}
//	}
//	else if ((*token)->type == REDIR_OUT || (*token)->type == APPEND)
//	{
//		*token = (*token)->next;
//		if (*token && (*token)->str)
//		{
//			file.file = ft_strdup((*token)->str);
//			file = file.next;
//			*token = (*token)->next;
//		}
//	}
//}

//
//t_data	cmd_builder(t_data *data)
//{
//	t_token	*token;
//	t_cmd	*cmd;
//	int i;
//
//	i = 0;
//	token = data->token;
//	data->cmd = cmd_list(token);
//	cmd = data->cmd;
//	while (token && cmd)
//	{
//		if (token->type == WORD)
//		{
//			cmd->cmd_param[i++] = ft_strdup(token->str);
//			token = token->next;
//		}
//		else if (token->type != WORD && token->type != PIPE)
//		{
//			handle_redir(cmd, &token);
//		}
//		else if (token->type == PIPE)
//		{
//			cmd->cmd_param = NULL;
//			cmd = cmd->next;
//			token = token->next;
//			i = 0;
//		}
//	}
//	if (cmd)
//		cmd->cmd_param[i] = NULL;
//	return (*data);
//}

//t_data	cmd_builder(t_data *data)
//{
//	int	i;
//	t_token *token;
//	t_cmd *cmd;
//	int	in_redir;
//
//
//	i = 0;
//	in_redir = 0;
//	token = data->token;
//	data->cmd = cmd_list(token);
//	cmd = data->cmd;
//	while (token && cmd)
//	{
//		if ( token->type == WORD && in_redir != 1)
//		{
//			cmd->cmd_param[i] = ft_strdup(token->str);
//			token = token->next;
//			i++;
//		}
//		else
//		{
//			if (token->type == WORD)
//				in_redir = 0;
//			else
//				in_redir = 1;
//			cmd->cmd_param[i] = NULL;
//			cmd = cmd->next;
//			token = token->next;
//			i = 0;
//		}
//	}
//	if (cmd)
//		cmd->cmd_param[i] = NULL;
//	return (*data);
//}
