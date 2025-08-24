/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:22:47 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/04 01:20:40 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_iteration_data(t_data *data)
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
}

void	close_and_free_all(t_data *data)
{
	if (data->fd[0] != -1)
		close(data->fd[0]);
	if (data->fd[1] != -1)
		close(data->fd[1]);
	if (data->prev_fd != -1)
		close(data->prev_fd);
	ft_close_save(data);
	ft_free_and_exit(*data, NULL);
}
