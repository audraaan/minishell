/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:11:10 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/19 05:14:18 by alarroye         ###   ########lyon.fr   */
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
			printf("%s\n", tmp_env->content);
		tmp_env = tmp_env->next;
	}
	return (0);
}

// void	ft_unset(t_lst **env, char **a)
//{
//	t_lst	*tmp_prev;
//	t_lst	*tmp_last;
//	int		i;

//	if (check_params_env(a, &i))
//		return ;
//	while (a && a[++i])
//	{
//		tmp_prev = *env;
//		tmp_last = (*env)->next;
//		if (ft_strncmp(tmp_prev->content, a[i], ft_strlen(a[i])) == 0)
//			ft_free_lst(*env);
//		else
//			while (tmp_last)
//			{
//				if (ft_strncmp(tmp_last->content, a[i], ft_strlen(a[i])) == 0)
//				{
//					tmp_prev->next = tmp_last->next;
//					ft_free_lst(tmp_last);
//					break ;
//				}
//				tmp_prev = tmp_prev->next;
//				tmp_last = tmp_last->next;
//			}
//	}
//}
char	**lst_in_tab(t_lst *env)
{
	int		len_env;
	char	**tab_env;
	int		i;

	len_env = ft_lstlen(env);
	tab_env = malloc(sizeof(char *) * (len_env + 1));
	i = -1;
	if (!tab_env)
		return (NULL);
	while (env)
	{
		tab_env[++i] = ft_strdup(env->content);
		if (!tab_env[i])
		{
			ft_free_dtab(tab_env);
			return (NULL);
		}
		env = env->next;
	}
	tab_env[len_env] = NULL;
	return (tab_env);
}

void	sort_tab(char **tab)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (tab && tab[++i])
	{
		j = i;
		while (tab[++j])
		{
			if (ft_strcmp(tab[i], tab[j]) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
		}
	}
}

int	export_not_args(t_lst **env)
{
	char	**tab_env;
	int		i;

	tab_env = lst_in_tab(*env);
	if (!tab_env)
		return (-1); // pb malloc or env empty
	sort_tab(tab_env);
	i = -1;
	while (tab_env && tab_env[++i])
		ft_printf("export %s\n", tab_env[i]);
	ft_free_dtab(tab_env);
	return (0);
}

int	exist(t_lst **env, char *a)
{
	t_lst	*tmp;
	int		i;

	i = 0;
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, a, (ft_strlen((*env)->content)
					- ft_strlen(ft_strchr((*env)->content, '=')))))
			// compare tmp->content avec l arg sur jusquau egal (strchr pr trouver le egale et je le soustrait a len de content :) oui il est 3h du mat
			return (i);
		tmp = tmp->next;
		i++;
	}
	return (-1);
}

int	check_params_env(char *a)
{
	int	i;

	i = 0;
	if (a[0] == '=')
		return (1);
	while (a[i] && (a[i] != '='))
	{
		if (!(ft_isalpha(a[i]) || a[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}

int	ft_export(t_lst **env, char **a)
{
	int		i;
	int		id;
	t_lst	*tmp;

	tmp = *env;
	i = -1;
	while (a && a[++i])
	{
		printf("a=%s;\n\n\n", a[i]);
		if (check_params_env(a[i]))
		{
			// ft_printf("export:");
			ft_error_msg(a[i], "not a valid identifier");
		}
		id = exist(env, a[i]);
		else if (id != -1)
		{
			while (id--)
				tmp = tmp->next;
			free(tmp->content);
			tmp->content = ft_strdup(a[i]);
			if (!tmp->content)
				return (ft_printf("error strdup malloc"));
		}
		else
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
