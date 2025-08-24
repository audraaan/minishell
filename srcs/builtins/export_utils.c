/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:55:41 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/04 01:12:33 by alarroye         ###   ########lyon.fr   */
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

t_list	*create_env_node_from_parts(char *name, char *content)
{
	t_list	*node;

	if (!name)
		return (NULL);
	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	if (content)
		node->content = ft_strdup(content);
	else
		node->content = NULL;
	node->next = NULL;
	if (content && (!node->name || !node->content))
	{
		free(node->name);
		free(node->content);
		free(node);
		return (NULL);
	}
	free(name);
	if (content)
		free(content);
	return (node);
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
		return (-2);
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
