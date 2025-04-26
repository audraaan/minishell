/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:58:17 by nbedouan          #+#    #+#             */
/*   Updated: 2025/04/26 11:00:21 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*char_to_str(char c)
{
	char	*str;
	str = malloc(2);
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
		return (NULL);
	str = ft_strjoin(s1, s2);
	free(s2);
	free(s1);
	return (str);
}

char	*get_env_value(t_list *env, char *name)
{
	while(env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

char *expand_env_var(t_list *env_cpy, char *str)
{
	int	i;
	int	quotes;
	int	start;
	char	*res;
	char	*name;
	char	*value;

	res = ft_strdup("");
	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' && quotes != 2)
		{
			quotes = quotes == 1 ? 0 : 1;
			res = join_and_free(res, char_to_str(str[i++]));
		}
		else if (str[i] == '\"' && quotes != 1)
		{
			quotes = quotes == 2 ? 0 : 2;
			res = join_and_free(res, char_to_str(str[i++]));
		}
		else if (str[i] == '$' && quotes != 1)
		{
			i++;
			start = i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			name = ft_substr(str, start, i - start);
			value = get_env_value(env_cpy, name);
			free (name);
			res = ft_strjoin(res, value);
		}
		else
		{
			res = join_and_free(res, char_to_str(str[i]));
			i++;
		}
	}
	return (res);
}

t_list *cpy_env(char **env)
{
	int	i;
	int	j;
	char *name;
	char *content;
	t_list *env_cpy;

	env_cpy = NULL;
	i = 0;
	j = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		name = ft_substr(env[i], 0, j);
		if (env[i][j] == '=')
			content = ft_strdup(&env[i][j + 1]);
		else
			content = NULL;
		ft_lstadd_back(&env_cpy, ft_lstnew(name, content));
		i++;
	}
	return (env_cpy);
}