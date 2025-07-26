/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:23:57 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 08:22:09 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_list *env)
{
	t_list	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->name)
			free(tmp->name);
		if (tmp->content)
			free(tmp->content);
		if (tmp)
			free(tmp);
	}
}

void	free_tokens(t_token **token)
{
	t_token	*tmp;

	if (!token || !*token)
		return ;
	while (*token)
	{
		tmp = (*token)->next;
		if ((*token)->str)
			free((*token)->str);
		free(*token);
		*token = tmp;
	}
	(*token) = NULL;
}

void	free_file_list(t_file **file)
{
	t_file	*tmp;

	if (!file || !*file)
		return ;
	while (*file)
	{
		tmp = (*file)->next;
		if ((*file)->eof)
		{
			if ((*file)->filename)
				unlink((*file)->filename);
			free((*file)->eof);
		}
		if ((*file)->filename)
			free((*file)->filename);
		free(*file);
		*file = tmp;
	}
	*file = NULL;
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	if (!cmd || !*cmd)
		return ;
	while ((*cmd))
	{
		tmp = (*cmd)->next;
		i = 0;
		if ((*cmd)->cmd_param)
		{
			while ((*cmd)->cmd_param[i])
			{
				free((*cmd)->cmd_param[i]);
				i++;
			}
			free((*cmd)->cmd_param);
		}
		if ((*cmd)->file)
			free_file_list(&(*cmd)->file);
		free(*cmd);
		*cmd = tmp;
	}
	*cmd = NULL;
}

void	free_all(t_data *data, char *read)
{
	if (data->token)
	{
		free_tokens(&data->token);
		data->token = NULL;
	}
	if (data->cmd)
	{
		free_cmd(&data->cmd);
		data->cmd = NULL;
	}
	if (data->env)
	{
		free_env(data->env);
		data->env = NULL;
	}
	if (read)
	{
		free(read);
		read = NULL;
	}
}
