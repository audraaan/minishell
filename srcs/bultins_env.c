/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:11:10 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/07 15:37:22 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_env(t_list *env)
{
	t_list	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if (tmp_env->content)
			ft_printf("%s=%s\n", tmp_env->name, tmp_env->content);
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

int	cmp_unset(t_list **prev, t_list **last, char *a)
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

int	ft_unset(t_list **env, char **a)
{
	t_list	*tmp;
	t_list	*prev;

	while (a && *a)
	{
		tmp = *env;
		prev = NULL;
		if (!check_params_env(*a) && !ft_strchr(*a, '='))
			while (tmp)
			{
				if (ft_strcmp(tmp->name, *a) == 0)
				{
					if (prev == NULL)
						*env = tmp->next;
					else
						prev->next = tmp->next;
					ft_free_lst(tmp);
					break ;
				}
				prev = tmp;
				tmp = tmp->next;
			}
		a++;
	}
	return (0);
}
