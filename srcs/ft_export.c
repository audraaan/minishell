/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:31:45 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/03 00:44:13 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_list	*sort_list(t_list *env)
{
	t_list	*lst;
	int		swapped;
	char	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		lst = env;
		while (lst->next)
		{
			if (ft_strcmp(lst->name, lst->next->name) > 0)
			{
				tmp = lst->name;
				lst->name = lst->next->name;
				lst->next->name = tmp;
				tmp = lst->content;
				lst->content = lst->next->content;
				lst->next->content = tmp;
				swapped = 1;
			}
			lst = lst->next;
		}
	}
	return (env);
}

int	export_not_args(t_list **env)
{
	t_list	*sorted;

	sorted = sort_list(*env);
	while (sorted)
	{
		ft_printf("export ");
		if (sorted->content)
			ft_printf("%s=\"%s\"\n", sorted->name, sorted->content);
		else
			ft_printf("%s\n", sorted->name);
		sorted = sorted->next;
	}
	return (0);
}

int	exist(t_list **env, char *a)
{
	t_list	*tmp;
	int		i;
	int		len;
	char	*arg;

	i = 0;
	tmp = *env;
	len = ft_strlen(a);
	if (ft_strchr(a, '='))
		len = ft_strlen(a) - ft_strlen(ft_strchr(a, '='));
	arg = ft_strndup(a, len);
	if (!arg)
		return (-1);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, arg) == 0)
		{
			free(arg);
			return (i);
		}
		tmp = tmp->next;
		i++;
	}
	free(arg);
	return (-1);
}

int	ft_change_var(t_list **env, char *a)
{
	int		pos;
	t_list	*tmp;
	int		len;

	tmp = *env;
	if (!ft_strchr(a, '='))
		return (0);
	pos = exist(env, a);
	len = ft_strlen(a) - ft_strlen(ft_strchr(a, '='));
	while (pos--)
		tmp = tmp->next;
	// ft_printf("tmp=%s,a=%s\n", tmp->name, a);
	free(tmp->content);
	tmp->content = ft_strdup(ft_strchr(a, '=') + 1);
	if (!tmp->content)
	{
		return (ft_printf("malloc failed\n"));
	}
	return (0);
}

int	ft_export(t_list **env, char **a)
{
	int		i;
	t_list	*tmp;

	tmp = *env;
	i = 0;
	if (!a || !a[1]) //! a[0] || peut etre ?
		return (export_not_args(env));
	while (a && a[++i])
	{
		if (check_params_env(a[i]))
			ft_printf("\nminishell: export: %s: not a valid identifier\n",
				a[i]);
		else if (exist(env, a[i]) != -1 && ft_change_var(env, a[i]))
			return (ft_printf("malloc failed\n"));
		else if (exist(env, a[i]) == -1)
		{
			tmp = ft_last_node(tmp);
			tmp->next = new_node(a[i]);
			if (!tmp->next)
				return (ft_printf("error strdup malloc"));
		}
		tmp = *env;
	}
	return (0);
}
