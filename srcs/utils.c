/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:09 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/01 11:26:09 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_error_msg(char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int	ft_error(char *msg, char **path, char **dtab, int status)
{
	if (dtab && dtab[0])
		ft_error_msg(dtab[0], msg);
	else
		ft_error_msg(" ", msg);
	ft_free_dtab(path);
	ft_free_dtab(dtab);
	if (status != -1)
		exit(status);
	return (1);
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

int	ft_is_exec(char *path_cmd, int *error)
{
	if (access(path_cmd, F_OK) == 0)
	{
		if (access(path_cmd, X_OK) == 0)
		{
			*error = 0;
			return (1);
		}
		else
		{
			*error = 126;
			return (1);
		}
	}
	else
		*error = 127;
	return (0);
}

void	ft_free_all_lst(t_lst *lst)
{
	t_lst	*tmp;

	while (lst)
	{
		tmp = lst->next;
		ft_free_lst(lst);
		lst = tmp;
	}
}

void	ft_free_lst(t_lst *lst)
{
	t_lst	*tmp;

	tmp = lst;
	lst = lst->next;
	if (tmp->content)
		free(tmp->content);
	if (tmp)
		free(tmp);
}

t_lst	*new_node(char *str)
{
	t_lst	*node;

	node = malloc(sizeof(t_lst));
	if (!node)
		return (NULL);
	node->content = ft_strdup(str);
	if (!(node->content))
		return (NULL);
	node->next = NULL;
	return (node);
}

int	ft_lstlen(t_lst *lst)
{
	size_t i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

t_lst	*ft_last_node(t_lst *lst)
{
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}
