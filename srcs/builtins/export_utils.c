/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:55:41 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/18 03:33:52 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//char	*expand_value(t_data *data, char *str)
//{
//	char	*expanded;
//	t_token	*current;
//
//	current = data->token;
//	expanded = expand_env_var(data, str, &current);
//	if (!expanded)
//		return (NULL);
//	return (expanded);
//}

t_list	*create_env_node_from_parts(char *name, char *content)
{
	t_list	*node;

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
	if (name)
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
