/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:31:45 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/20 07:20:13 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_not_env(t_list **env, char **a, t_data *data)
{
	char	*name;
	char	*content;
	int		len;

	if (check_params_env(a[1]))
	{
		data->exit_status = ft_error_msg(a[1],
				"not a valid identifier for export");
		return (1);
	}
	if (ft_strchr(a[1], '='))
	{
		len = ft_strlen(ft_strchr(a[1], '='));
		name = ft_strndup(a[1], ft_strlen(a[1]) - len);
		content = ft_strdup((ft_strchr(a[1], '=') + 1));
	}
	else
	{
		name = ft_strdup(a[1]);
		content = NULL;
	}
	env[0] = create_env_node_from_parts(name, content);
	a++;
	return (0);
}

int	ft_export(t_list **env, char **a, t_data *data)
{
	int		i;
	t_list	*tmp;

	i = 0;
	if (!a || !(*a) || !a[1])
		return (export_not_args(env));
	if (!*env && ft_not_env(env, a, data))
		return (data->exit_status);
	tmp = *env;
	while (a && a[++i])
	{
		if (check_params_env(a[i]))
			data->exit_status = ft_error_msg(a[i],
					"not a valid identifier for export");
		else if (exist(env, a[i]) != -1 && ft_change_var(env, a[i], data))
			return (ft_error_msg(NULL, "malloc failed"));
		else if (exist(env, a[i]) == -1)
			ft_export_bis(tmp, data, a, &i);
		tmp = *env;
	}
	return (data->exit_status);
}

void	ft_export_bis(t_list *tmp, t_data *data, char **a, int *i)
{
	char	*name;
	char	*value;
	char	*equal_pos;
	int		name_len;

	equal_pos = ft_strchr(a[(*i)], '=');
	tmp = ft_last_node(tmp);
	if (equal_pos)
	{
		name_len = equal_pos - a[(*i)];
		name = ft_strndup(a[(*i)], name_len);
		if (!name)
			ft_error_msg("ft_strndup", "malloc failed");
		value = ft_strdup(equal_pos + 1);
		if (!value)
		{
			free(name);
			ft_error_msg("expand_env_var", "malloc failed");
		}
		tmp->next = create_env_node_from_parts(name, value);
	}
	else
		tmp->next = new_node(a[(*i)]);
	if (!tmp->next)
		ft_error_msg("malloc", "malloc failed");
}

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

	if (!(*env))
		return (0);
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

int	ft_change_var(t_list **env, char *a, t_data *data)
{
	int		pos;
	t_list	*tmp;
	char	*value;
	char	*equal_pos;

	equal_pos = ft_strchr(a, '=');
	tmp = *env;
	if (!ft_strchr(a, '='))
		return (0);
	pos = exist(env, a);
	while (pos--)
		tmp = tmp->next;
	value = ft_strdup(equal_pos + 1);
	if (!value)
		return (ft_error_msg("", "malloc failed"));
	free(tmp->content);
	tmp->content = value;
	return (0);
}
