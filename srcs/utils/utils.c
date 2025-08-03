/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:09 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/02 22:03:46 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_str_isdigit(char *arg)
{
	char	*str;

	str = arg;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

t_list	*new_node(char *str)
{
	t_list	*node;
	int		len;

	len = ft_strlen(str) - ft_strlen(ft_strchr(str, '='));
	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	if (ft_strchr(str, '=') && (ft_strchr(str, '=') + 1))
	{
		node->content = ft_strdup(ft_strchr(str, '=') + 1);
		if (!(node->content))
			return (NULL);
	}
	else
		node->content = NULL;
	node->name = ft_strndup(str, len);
	if (!(node->name))
	{
		free(node->content);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

int	ft_cmdlen(t_cmd *cmd)
{
	size_t	i;
	t_cmd	*tmp;

	tmp = cmd;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

t_list	*ft_last_node(t_list *lst)
{
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}

int	check_synthax(t_data *data)
{
	t_token	*token;

	token = data->token;
	if (!(token->str))
		return (er_msg_free_tok(NULL, "command not found", &data->token));
	if (token->type == PIPE)
		return (er_msg_free_tok(token->str,
				"syntax error near unexpected token", &data->token));
	while (token)
	{
		if ((!token->str || token->str[0] == '\0'))
			return (er_msg_free_tok(token->str, "command not found",
					&data->token));
		if (token->type == PIPE && (!token->next || token->next->type == PIPE))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		else if (!(token->type == PIPE || token->type == WORD) && (!token->next
				|| token->next->type != WORD))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		token = token->next;
	}
	return (0);
}

int	ft_lstlen(t_list *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

int	er_msg_free_tok(char *arg, char *msg, t_token **token)
{
	char *tmp;
	int res;

	tmp = NULL;
	if (arg)
		tmp = ft_strdup(arg);
	if (token)
	{
		free_tokens(token);
		token = NULL;
	}
	res = ft_error_msg(tmp, msg);
	if (tmp && *tmp)
		free(tmp);
	return (res);
}
