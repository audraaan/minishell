/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:58:17 by nbedouan          #+#    #+#             */
/*   Updated: 2025/04/23 17:00:13 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_env_var(t_data *data, char *str, t_token **current)
{
	char	*res;

	if (!current || !(*current) || !check_token(current))
		return (ft_strdup(str));
	res = ft_strdup("");
	if (!res)
		return (NULL);
	expand_env_var_bis(data, str, &res, current);
	if ((*current)->retokenized)
		return (free_return(res, NULL));
	return (res);
}

void	expand_env_var_bis(t_data *data, char *str, char **res,
						t_token **current)
{
	int		i;
	int		quotes;

	quotes = 0;
	i = 0;
	while (str[i])
	{
		if (handle_quote(&i, &quotes, str, (*current)->q_type))
		{
			i++;
			continue ;
		}
		if (!str[i])
			break ;
		if (str[i] == '$' && str[i + 1] == '?' && quotes != 1)
			manage_exit_status(&data, &i, str, res);
		else if (str[i] == '$' && quotes != 1)
		{
			data->current_token = current;
			expend_env_var_third(&i, str, data, res);
			if ((*current)->retokenized)
				return ;
		}
		else
		{
			*res = join_and_free(*res, char_to_str(str[i++]));
		}
	}
}

void	expend_env_var_third(int *i, char *str, t_data *data, char **res)
{
	char	*name;
	char	*value;
	int		start;
	t_token	**current;

	current = (data->current_token);
	(*i)++;
	start = (*i);
	while (str[(*i)] && (ft_isalnum(str[(*i)]) || str[(*i)] == '_'))
		(*i)++;
	name = ft_substr(str, start, (*i) - start);
	if (!name || name[0] == '\0')
	{
		handle_empty_var(res);
		free(name);
		return ;
	}
	value = get_env_value(data->env, name);
	(*current)->expanded = 1;
	free(name);
	if (!value)
		return ;
	if ((*current)->q_type == NO_QUOTES && needs_space_splitting(value))
		handle_retoken(data, value, current, res);
	else
		*res = join_and_free(*res, ft_strdup(value));
}

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

//static char *get_remaining_str(char *str)
//{
//	int i = 0;
//
//	while (str[i] && str[i] != '$')
//		i++;
//	if (!str[i])
//		return (ft_strdup(""));
//	i++;
//	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
//		i++;
//	return (ft_substr(str, i, ft_strlen(str) - i));
//}
//
//void	handle_retoken(t_data *data, char *value, t_token **current, char **res)
//{
//	t_token	*new_tokens;
//	char	*combined_value;
//	char	*remaining_str;
//	char	*tmp;
//
//	remaining_str = get_remaining_str((*current)->str);
//	if (*res && **res)
//	{
//		tmp = ft_strjoin(*res, value);
//		if (!tmp)
//			return ;
//		if (remaining_str)
//			combined_value = ft_strjoin(tmp, remaining_str);
//		else
//			combined_value = tmp;
//	}
//	else
//		combined_value = ft_strdup(value);
//	new_tokens = retokenize(data, combined_value, (*current)->q_type);
//	if (new_tokens)
//	{
//		replace_current_token_with_list(data, current, new_tokens);
//		(*current)->retokenized = 1;
//	}
//	else
//	{
//		tmp = ft_strdup(combined_value);
//		if (tmp)
//			*res = join_and_free(*res, tmp);
//		(*current)->retokenized = 0;
//	}
//	free(combined_value);
//}
