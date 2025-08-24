/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:57:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:33:03 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*char_to_str(char c)
{
	char	*str;

	str = malloc(sizeof(char) * 2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*str;

	if (!s1 || !s2)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	str = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (str);
}

char	*get_env_value(t_list *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

int	check_unclosed_quotes(t_quote_type q_type)
{
	if (q_type != 0)
	{
		return (1);
	}
	return (0);
}

t_list	*create_env_node(char *env_var, t_list **env_cpy)
{
	int		j;
	char	*name;
	char	*content;

	j = 0;
	while (env_var[j] && env_var[j] != '=')
		j++;
	name = ft_substr(env_var, 0, j);
	if (!name)
	{
		free_env(*env_cpy);
		return (NULL);
	}
	if (env_var[j] == '=')
		content = ft_strdup(&env_var[j + 1]);
	else
		content = NULL;
	if (env_var[j] == '=' && !content)
	{
		free(name);
		free_env(*env_cpy);
		return (NULL);
	}
	return (ft_lstnew(name, content));
}
