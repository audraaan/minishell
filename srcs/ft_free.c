/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:22:47 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/17 05:10:04 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_env(t_list *env)
{
	t_list	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->content);
		free(tmp);
	}
	env = NULL;
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
	token = NULL;
}

void	free_all(t_data data, char *read)
{
	if (data.token)
		free_tokens(data.token);
	if (data.env)
		free_env(data.env);
	if (data.cmd)
		free_cmd(data.cmd);
	if (read && *read)
		free(read);
}

void	ft_free_all_lst(t_list *lst)
{
	t_list	*tmp;

	while (lst)
	{
		tmp = lst->next;
		ft_free_lst(lst);
		lst = tmp;
	}
}

void	ft_free_lst(t_list *lst)
{
	t_list	*tmp;

	tmp = lst;
	lst = lst->next;
	if (tmp->name)
		free(tmp->name);
	if (tmp->content)
		free(tmp->content);
	if (tmp)
		free(tmp);
}

int	ft_free_dtab(char **tab)
{
	int	i;

	i = -1;
	while (tab && tab[++i])
		free(tab[i]);
	if (tab)
		free(tab);
	return (1);
}

void	free_file_list(t_file *file)
{
	t_file	*tmp;

	while (file)
	{
		tmp = file;
		file = file->next;
		if (tmp->file)
			free(tmp->file);
		if (tmp->eof)
			free(tmp->eof);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		if (tmp->cmd_param)
		{
			i = 0;
			while (tmp->cmd_param[i])
			{
				free(tmp->cmd_param[i]);
				i++;
			}
			if (tmp->cmd_param)
				free(tmp->cmd_param);
		}
		if (tmp->file)
			free_file_list(tmp->file);
		free(tmp);
	}
}

void	free_iteration_data(t_data *data)
{
	if (data->token)
	{
		free_tokens(data->token);
		data->token = NULL;
	}
	if (data->cmd)
	{
		free_cmd(data->cmd);
		data->cmd = NULL;
	}
}