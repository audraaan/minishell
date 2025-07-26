/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:58:17 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 10:35:10 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*cpy_env(char **env, t_data *data)
{
	int		i;
	t_list	*env_cpy;
	t_list	*new_node;

	env_cpy = NULL;
	i = 0;
	while (env && env[i])
	{
		if (env[i])
			new_node = create_env_node(env[i], &env_cpy);
		if (!new_node)
			return (NULL);
		ft_lstadd_back(&env_cpy, new_node);
		i++;
	}
	if (!env_cpy)
		env_cpy = ft_lstnew(ft_strdup("OLDPWD"), NULL);
	if (ft_make_env(&env_cpy, data))
	{
		ft_free_all_lst(env_cpy);
		return (NULL);
	}
	return (env_cpy);
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

char	*expand_env_var(t_data *data, char *str)
{
	int		i;
	int		quotes;
	char	*res;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (handle_quote(&i, &quotes, str))
			continue ;
		if (!str[i])
			break ;
		if (str[i] == '$' && str[i + 1] == '?' && quotes != 1)
			manage_exit_status(&data, &i, str, &res);
		else if (str[i] == '$' && quotes != 1)
			expend_env_var_bis(&i, str, data->env, &res);
		else
			res = join_and_free(res, char_to_str(str[i++]));
	}
	if (check_unclosed_quotes(quotes))
	{
		free(res);
		return (NULL);
	}
	return (res);
}

int	handle_quote(int *i, int *quotes, char *str)
{
	if (str[(*i)] == '\'' && (*quotes) != 2)
	{
		if ((*quotes) == 1)
			(*quotes) = 0;
		else
			(*quotes) = 1;
		(*i)++;
		return (1);
	}
	else if (str[(*i)] == '\"' && (*quotes) != 1)
	{
		if ((*quotes) == 2)
			(*quotes) = 0;
		else
			(*quotes) = 2;
		(*i)++;
		return (1);
	}
	return (0);
}

void	expend_env_var_bis(int *i, char *str, t_list *env_cpy, char **res)
{
	char	*name;
	char	*value;
	int		start;

	(*i)++;
	start = (*i);
	while (str[(*i)] && (ft_isalnum(str[(*i)]) || str[(*i)] == '_'))
		(*i)++;
	name = ft_substr(str, start, (*i) - start);
	if (!name || name[0] == '\0')
	{
		free(name);
		*res = join_and_free(*res, ft_strdup("$"));
		return ;
	}
	value = get_env_value(env_cpy, name);
	if (value)
		*res = join_and_free(*res, ft_strdup(value));
	free(name);
}
