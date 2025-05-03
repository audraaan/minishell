/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:11:10 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/03 17:06:41 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_env(t_lst *env)
{
	t_lst	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if (ft_strchr(tmp_env->content, '='))
			ft_printf("%s\n", tmp_env->content);
		tmp_env = tmp_env->next;
	}
	return (0);
}

int	check_params_env(char *a)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(a[0]) || a[0] == '_'))
		return (1);
	while (a[i] && (a[i] != '='))
	{
		if (!(ft_isalnum(a[i]) || a[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}

int	cmp_unset(t_lst **prev, t_lst **last, char *a)
{
	int	len;
	int	len_a;

	len_a = ft_strlen(a);
	len = ft_strlen((*last)->content);
	if (ft_strchr(a, '='))
		len_a = ft_strlen(a) - ft_strlen(ft_strchr(a, '='));
	if (ft_strchr((*last)->content, '='))
		len = ft_strlen((*last)->content)
			- ft_strlen(ft_strchr((*last)->content, '='));
	if (len_a > len)
		len = len_a;
	if (ft_strncmp((*last)->content, a, len) == 0)
	{
		(*prev)->next = (*last)->next;
		ft_free_lst(*last);
		return (1);
	}
	else
		return (0);
}

int	ft_unset(t_lst **env, char **a)
{
	t_lst	*tmp_prev;
	t_lst	*tmp_last;
	int		i;

	i = -1;
	while (a && a[++i])
	{
		if (!check_params_env(a[i]) && !ft_strchr(a[i], '='))
		{
			tmp_prev = *env;
			tmp_last = (*env)->next;
			if (ft_strncmp(tmp_prev->content, a[i], ft_strlen(a[i])) == 0)
				ft_free_lst(*env);
			else
				while (tmp_last)
				{
					if (cmp_unset(&tmp_prev, &tmp_last, a[i]))
						break ;
					tmp_prev = tmp_prev->next;
					tmp_last = tmp_last->next;
				}
		}
	}
	return (0);
}
