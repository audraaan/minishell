/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:55:41 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/27 21:55:45 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_list	*create_env_node_from_parts(char *name, char *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	node->content = ft_strdup(content);
	node->next = NULL;
	if (!node->name || !node->content)
	{
		free(node->name);
		free(node->content);
		free(node);
		return (NULL);
	}
	free(name);
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
