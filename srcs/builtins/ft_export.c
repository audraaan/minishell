/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:31:45 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/27 02:15:13 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_list **env, char **a, t_data *data)
{
	int		i;
	t_list	*tmp;

	tmp = *env;
	i = 0;
	if (!a || !a[1])
		return (export_not_args(env));
	while (a && a[++i])
	{
		if (check_params_env(a[i]))
			data->exit_status = ft_error_msg(a[i],
					"not a valid identifier for export");
		else if (exist(env, a[i]) != -1 && ft_change_var(env, a[i], data))
			return (ft_error_msg(NULL, "malloc failed"));
		else if (exist(env, a[i]) == -1)
		{
			ft_export_bis(tmp, data, a, &i);
		}
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
		value = expand_value(data, equal_pos + 1);
		if (!value)
		{
			free(name);
			ft_error_msg("expand_env_var", "malloc failed");
		}
		tmp->next = create_env_node_from_parts(name, value);
		free(name);
		free(value);
	}
	else
		tmp->next = new_node(a[(*i)]);
	if (!tmp->next)
		ft_error_msg("malloc", "malloc failed");
}


char	*expand_value(t_data *data, char *str)
{
	char	*no_quotes;
	char	*expanded;

	no_quotes = remove_outer_quotes(str);
	if (!no_quotes)
		return (NULL);
	expanded = expand_env_var(data, no_quotes);
	free(no_quotes);
	if (!expanded)
		return (NULL);
	return (expanded);
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
	value = remove_outer_quotes(expand_value(data, equal_pos + 1));
	free(tmp->content);
	tmp->content = value;
	if (!tmp->content)
		return (ft_error_msg("ft_strdup", "malloc failed"));
	return (0);
}

t_list *create_env_node_from_parts(char *name, char *content)
{
	t_list *node = malloc(sizeof(t_list));
	if (!node)
		return NULL;
	node->name = ft_strdup(name);
	node->content = ft_strdup(content);
	node->next = NULL;
	if (!node->name || !node->content)
	{
		free(node->name);
		free(node->content);
		free(node);
		return NULL;
	}
	return node;
}

