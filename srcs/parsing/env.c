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

//char	*expand_env_var(t_data *data, char *str)
//{
//	int		quotes;
//	char	*res;
//
//	res = ft_strdup("");
//	if (!res)
//		return (NULL);
//	quotes = 0;
//	expand_env_var_bis(data, &quotes, str, &res);
//	if (check_unclosed_quotes(quotes) && res)
//	{
//		free(res);
//		return (NULL);
//	}
//	return (res);
//}
//
//void	expand_env_var_bis(t_data *data, int *quotes, char *str, char **res)
//{
//	int	i;
//
//	i = 0;
//	printf("%s\n", str);
//	while (str[i])
//	{
//		if (handle_quote(&i, quotes, str))
//			continue ;
//		if (!str[i])
//			break ;
//		if (str[i] == '$' && str[i + 1] == '?' && *quotes != 1)
//			manage_exit_status(&data, &i, str, res);
//		else if (str[i] == '$' && *quotes != 1)
//			expend_env_var_third(&i, str, data->env, res);
//		else
//			*res = join_and_free((*res), char_to_str(str[i++]));
//	}
//}

char	*expand_env_var(t_data *data, char *str, t_token **current)
{
	int		quotes;
	char	*res;
	int		retokenized;
	char	*tmp;

	quotes = 0;
	retokenized = 0;
	res = ft_strdup("");
	tmp = ft_strdup(str);
	if (!tmp)
	{
		free(res);
		free(tmp);
		return (NULL);
	}
	expand_env_var_bis(data, &quotes, tmp, &res, current, &retokenized);
	free(tmp);
	if (check_unclosed_quotes(quotes))
	{
		free(res);
		return (NULL);
	}
	if (retokenized)
	{
		free(res);
		return (NULL);
	}
	return (res);
}

void	expand_env_var_bis(t_data *data, int *quotes, char *str, char **res, t_token **current, int *retokenized)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (handle_quote(&i, quotes, str))
			continue;
		if (!str[i])
			break;
		if (str[i] == '$' && str[i + 1] == '?' && *quotes != 1)
			manage_exit_status(&data, &i, str, res);
		else if (str[i] == '$' && *quotes != 1)
			expend_env_var_third(&i, str, data->env, data, res, current, quotes, retokenized);
		else
			*res = join_and_free(*res, char_to_str(str[i++]));
	}
}

static int	needs_space_splitting(char *value)
{
	int i = 0;

	if (!value)
		return (0);
	while (value[i])
	{
		if (ft_isspace(value[i]) || is_operator(value[i]))
			return (1);
		i++;
	}
	return (0);
}

extern void	expend_env_var_third(int *i, char *str, t_list *env_cpy, t_data *data,
								 char **res, t_token **current, int *quotes, int *retokenized)
{
	char	*name;
	char	*value;
	char	*tmp;
	int		start;

	(*i)++;
	start = (*i);
	while (str[(*i)] && (ft_isalnum(str[(*i)]) || str[(*i)] == '_'))
		(*i)++;
	name = ft_substr(str, start, (*i) - start);
	if (!name || name[0] == '\0')
	{
		tmp = ft_strdup("$");
		if (tmp)
			*res = join_and_free(*res, tmp);
		free(name);
		return;
	}
	value = get_env_value(env_cpy, name);
	free(name);
	if (value)
	{
		if (*quotes == 0 && needs_space_splitting(value))
		{
			*retokenized = 1;
			if (*res && **res)
				*res = join_and_free(*res, ft_strdup(""));
			t_token *new_tokens = tokenize(data, value);
			if (new_tokens)
			{
				replace_current_token_with_list(data, current, new_tokens);
				if (new_tokens->type != WORD)
				{
					new_tokens->type = WORD;
//					ft_error_msg(new_tokens->str, "command not found");
//					free_all(data, NULL);
//					free(*res);
//					free(str);
//					exit(127);
				}
			}
			else
			{
				tmp = ft_strdup(value);
				if (tmp)
					*res = join_and_free(*res, tmp);
				*retokenized = 0;
			}
		}
		else
		{
			tmp = ft_strdup(value);
			if (tmp)
				*res = join_and_free(*res, tmp);
		}
	}
}


//extern void	expend_env_var_third(int *i, char *str, t_list *env_cpy, char **res, int *quotes)
//{
//	char	*name;
//	char	*value;
//	char	*tmp;
//	int		start;
//
//	(*i)++;
//	start = (*i);
//	tmp = NULL;
//	while (str[(*i)] && (ft_isalnum(str[(*i)]) || str[(*i)] == '_'))
//		(*i)++;
//	name = ft_substr(str, start, (*i) - start);
//	if (!name || name[0] == '\0')
//	{
//		tmp = ft_strdup("$");
//		if (tmp)
//		{
//			if (*quotes == 0)
//			{
//				t_token	*new_tokens;
//				new_tokens = tokenize(data, cleaned);
//
//				if (ft_lstlen(new_token) == 1)
//				{
//					*res = join_and_free(*res, new_token->str);
//				}
//				if (ft_lstlen(new_token) == 2)
//				{
//					*res = join_and_free(*res, new_token->str);
//					t_token *new = create_token(*res, WORD, 0);
//					new->next = current->next;
//					current->next = new;
////					current = new;
//					*res = join_and_free("", new_token->next->str);
//				}
//				if (ft_lstlen(new_token) > 2)
//				{
//					*res = join_and_free(*res, new_token->str);
//					t_token *new = create_token(*res, WORD, 0);
//					new->next = current->next;
//					current->next = new;
//
//
//					*res = join_and_free("", new_token->next->str);
//				}
//
//
//
//
//
//
//			}
//			else
//				*res = join_and_free(*res, tmp);
//		}
//		return (free(name));
//	}
//	value = get_env_value(env_cpy, name);
//	if (value)
//	{
//		tmp = ft_strdup(value);
//		if (tmp)
//			*res = join_and_free(*res, tmp);
//	}
//	return (free(name));
//}

//extern void	expend_env_var_third(int *i, char *str, t_list *env_cpy, char **res)
//{
//	char	*name;
//	char	*value;
//	char	*tmp;
//	int		start;
//
//	(*i)++;
//	start = (*i);
//	tmp = NULL;
//	while (str[(*i)] && (ft_isalnum(str[(*i)]) || str[(*i)] == '_'))
//		(*i)++;
//	name = ft_substr(str, start, (*i) - start);
//	if (!name || name[0] == '\0')
//	{
//		tmp = ft_strdup("$");
//		if (tmp)
//			*res = join_and_free(*res, tmp);
//		return (free(name));
//	}
//	value = get_env_value(env_cpy, name);
//	if (value)
//	{
//		tmp = ft_strdup(value);
//		if (tmp)
//			*res = join_and_free(*res, tmp);
//	}
//	return (free(name));
//}

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
