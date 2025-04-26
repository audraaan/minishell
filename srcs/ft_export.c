/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:31:45 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/21 03:42:26 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	exist(t_lst **env, char *a)
{
	t_lst	*tmp;
	int		i;
	int		len_tmp;
	int		len;

	i = 0;
	tmp = *env;
	while (tmp)
	{
		len_tmp = ft_strlen(a);
		len = ft_strlen(tmp->content);
		if (ft_strchr(a, '='))
			len_tmp = ft_strlen(a) - ft_strlen(ft_strchr(a, '='));
		if (ft_strchr(tmp->content, '='))
			len = ft_strlen(tmp->content) - ft_strlen(ft_strchr(tmp->content,
						'='));
		if (len_tmp > len)
			len = len_tmp;
		if (ft_strncmp(tmp->content, a, len) == 0)
			return (i);
		tmp = tmp->next;
		i++;
	}
	return (-1);
}

int	ft_change_var(t_lst **env, char *a)
{
	int		pos;
	t_lst	*tmp;

	tmp = *env;
	if (!strchr(a, '='))
		return (0);
	pos = exist(env, a);
	while (pos--)
		tmp = tmp->next;
	free(tmp->content);
	tmp->content = ft_strdup(a);
	if (!tmp->content)
		return (1);
	return (0);
}

int	ft_export(t_lst **env, char **a)
{
	int		i;
	t_lst	*tmp;

	tmp = *env;
	i = -1;
	if (!a)
		return (export_not_args(env));
	while (a && a[++i])
	{
		if (check_params_env(a[i]))
			ft_printf("\nminishell: export: %s: not a valid identifier\n",
				a[i]);
		else if (exist(env, a[i]) != -1 && ft_change_var(env, a[i]))
			return (ft_printf("error strdup malloc"));
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
