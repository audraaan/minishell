/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_not_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:33:54 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/21 03:38:18 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	print_var(char *t)
{
	int	i;
	int	limit;

	i = -1;
	limit = (ft_strlen(t) - ft_strlen(ft_strchr(t, '=')) + 1);
	while (limit--)
		ft_printf("%c", t[++i]);
	ft_printf("\"");
	while (t[i++])
		ft_printf("%c", t[i]);
	ft_printf("\"\n");
}

int	export_not_args(t_lst **env)
{
	char	**t;
	int		i;

	t = lst_in_tab(*env);
	if (!t)
		return (-1); // pb malloc or env empty
	sort_tab(t);
	i = -1;
	while (t && t[++i])
	{
		ft_printf("export ");
		if (!ft_strchr(t[i], '='))
			ft_printf("%s\n", t[i]);
		else
			print_var(t[i]);
	}
	ft_free_dtab(t);
	return (0);
}
