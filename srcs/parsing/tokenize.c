/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:39:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/05/01 08:20:47 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_data(t_data *data, int ac, char **av)
{
	(void)ac;
	(void)av;
	data->cmd = NULL;
	data->env = NULL;
	data->env = NULL;
	data->next = NULL;
	
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

char *extract_word(char *str, int *i)
{
	int	start;
	int	quotes;

	start = *i;
	quotes = 0;
	while (str[*i] && (quotes || (!is_operator(str[*i]) && !ft_isspace(str[*i]))))
	{
		if (str[*i] == '\'' && quotes != 2)
			quotes = quotes == 1 ? 0 : 1;
		else if (str[*i] == '\"' && quotes != 1)
			quotes = quotes == 2 ? 0 : 2;
		(*i)++;
	}
	return (ft_substr(str, start, *i - start));
}

t_token_type get_operator_type(char *str, int *i)
{
	if (str[(*i)] == '<' && str[(*i)+ 1] == '<')
	{
		(*i) += 2;
		return (HEREDOC);
	}
	else if (str[(*i)] == '>' && str[(*i)+ 1] == '>')
	{
		(*i) += 2;
		return (APPEND);
	}
	else if (str[(*i)] == '>')
	{
		(*i)++;
		return (REDIR_OUT);
	}
	else if (str[(*i)] == '<')
	{
		(*i)++;
		return (REDIR_IN);
	}
	else if (str[(*i)] == '|')
	{
		(*i)++;
		return (PIPE);
	}
	return (WORD);
}

char	*get_operator_str(t_token_type type)
{
	if (type == HEREDOC)
		return (ft_strdup("<<"));
	if (type == APPEND)
		return (ft_strdup(">>"));
	if (type == REDIR_IN)
		return (ft_strdup("<"));
	if (type == REDIR_OUT)
		return (ft_strdup(">"));
	if (type == PIPE)
		return (ft_strdup("|"));
	return (NULL);
}

t_token	*create_token(char *str, t_token_type type)
{
	t_token *new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	if (!new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

t_token	*tokenize(t_data *data, char *str)
{
	t_token *new_token;
	t_token **current;
	t_token_type	type;
	char	*word;
	int	i;

	new_token = NULL;
	current = &new_token;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if(!str[i])
			break ;
		if (is_operator(str[i]))
		{
			type = get_operator_type(str, &i);
			*current = create_token(get_operator_str(type), type);
		}
		else
		{
			type = WORD;
			word = extract_word(str, &i);
			*current = create_token(word, type);
			free(word);
		}
		if(!*current)
		{
			free_tokens(new_token);
			return (NULL);
		}
		current = &(*current)->next;
	}
	current = NULL;
	return (new_token);
}


void	print_tokens(t_token *head)
{
	const char	*type_names[] = {
			"REDIR_IN",
			"REDIR_OUT",
			"HEREDOC",
			"APPEND",
			"PIPE",
			"WORD"
	};

	printf("\n--- TOKENS ---\n");
	while (head)
	{
		printf("[%-8s] %s\n", type_names[head->type], head->str);
		head = head->next;
	}
	printf("--------------\n\n");
}

void	free_env(t_list *env)
{
	t_list *tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->content);
		free(tmp);
	}
}

void	free_tokens(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
	token = NULL;
}

void	free_all(t_data data, char *read)
{
	if (data.token)
		free_tokens(data.token);
	if (data.env)
		free_env(data.env);
	free(read);
}

