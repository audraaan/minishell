/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:22:47 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/19 21:06:55 by alarroye         ###   ########lyon.fr   */
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
