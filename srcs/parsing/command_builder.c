/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 22:02:16 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 00:33:16 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_count(t_token *token, int *nb_pipe)
{
	t_token *tmp;

	if (!token)
		return ;
	tmp = token;
	while(tmp)
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
	int	nb_param;

	nb_param = 1;
	if (!token)
		return (0);
	tmp = token;
	while(tmp && tmp->type != PIPE)
	{
		if (is_redirection(tmp->type))
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
			continue ;
		}
		if (tmp->type == WORD )
			nb_param++;
		tmp = tmp->next;
	}
	return (nb_param);
}

t_cmd *create_new_cmd(t_token *token)
{
	t_cmd	*new_cmd;
	int		param_size;
	int		i;

	i = 0;
	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	param_size = get_cmd_size(token);
	if (param_size >= 0)
		new_cmd->cmd_param = malloc(sizeof(char *) * (param_size + 1));
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

t_cmd *cmd_list(t_token *token)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*current_token;
	int	nb_pipe;
	int	i;

	i = 0;
	nb_pipe = 0;
	head = NULL;
	current = NULL;
	current_token = token;
	cmd_count(token, &nb_pipe);
	while (i <= nb_pipe)
	{
		t_cmd *new_cmd = create_new_cmd(current_token);
		if (!new_cmd)
		{
//			free_cmd_list(head);
			return (NULL);
		}
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		i++;
		while (current_token && current_token->type != PIPE)
			current_token = current_token->next;
		if (current_token)
			current_token = current_token->next;
	}
	return (head);
}

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

static t_file	*file_add_back(t_file **lst)
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

static void	copy_filename(t_file *current, t_token **token)
{
	*token = (*token)->next;
	if (*token && (*token)->str)
	{
		current->file = ft_strdup((*token)->str);
		*token = (*token)->next;
	}
}

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
		|| current->type == HEREDOC
		|| current->type == REDIR_OUT
		|| current->type == APPEND)
	{
		copy_filename(current, token);
	}
}


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

t_data cmd_builder(t_data *data)
{
	t_token *token;
	t_cmd *current_cmd;
	int param_index;

	param_index = 0;
	if (!data || !data->token)
		return (*data);
	data->cmd = cmd_list(data->token);
	if (!data->cmd)
		return (*data);
	current_cmd = data->cmd;
	token = data->token;
	while (token)
	{
		if (token->type == WORD)
		{
			if (current_cmd && current_cmd->cmd_param)
			{
				current_cmd->cmd_param[param_index] = ft_strdup(token->str);
				param_index++;
			}
			token = token->next;
		}
		else if (is_redirection(token->type))
			handle_redirection(&current_cmd->file, &token);
		else if (token->type == PIPE)
		{
			if (current_cmd && current_cmd->cmd_param)
				current_cmd->cmd_param[param_index] = NULL;
			current_cmd = current_cmd->next;
			param_index = 0;
			token = token->next;
		}
	}
	if (current_cmd && current_cmd->cmd_param)
		current_cmd->cmd_param[param_index] = NULL;
	return (*data);
}

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
