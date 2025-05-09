/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:09 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/09 18:25:38 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_error_msg(char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int	ft_error(char *msg, char **path, char **dtab, int status)
{
	if (dtab && dtab[0])
		ft_error_msg(dtab[0], msg);
	else
		ft_error_msg(" ", msg);
	ft_free_dtab(path);
	ft_free_dtab(dtab);
	if (status != -1)
		exit(status);
	return (1);
}

int	ft_free_dtab(char **tab)
{
	int	i;

	i = -1;
	while (tab && tab[++i])
		free(tab[i]);
	if (tab)
		free(tab);
	return (1);
}

int	ft_is_exec(char *path_cmd, int *error)
{
	if (access(path_cmd, F_OK) == 0)
	{
		if (access(path_cmd, X_OK) == 0)
		{
			*error = 0;
			return (1);
		}
		else
		{
			*error = 126;
			return (1);
		}
	}
	else
		*error = 127;
	return (0);
}

void	ft_free_all_lst(t_list *lst)
{
	t_list	*tmp;

	while (lst)
	{
		tmp = lst->next;
		ft_free_lst(lst);
		lst = tmp;
	}
}

void	ft_free_lst(t_list *lst)
{
	t_list	*tmp;

	tmp = lst;
	lst = lst->next;
	if (tmp->name)
		free(tmp->name);
	if (tmp->content)
		free(tmp->content);
	if (tmp)
		free(tmp);
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

t_list	*ft_last_node(t_list *lst)
{
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}

void	print_tokens(t_token *head)
{
	const char	*type_names[] = {"REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND",
			"PIPE", "WORD"};

	printf("\n--- TOKENS ---\n");
	while (head)
	{
		printf("[%-8s] %s\n", type_names[head->type], head->str);
		head = head->next;
	}
	printf("--------------\n\n");
}

t_list	*cpy_env(char **env)
{
	int		i;
	int		j;
	char	*name;
	char	*content;
	t_list	*env_cpy;

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

void	free_env(t_list *env)
{
	t_list	*tmp;

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
	t_token	*tmp;

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

char	**lst_in_tab(t_list *env)
{
	int len_env;
	char **tab_env;
	int i;

	len_env = ft_lstlen(env);
	tab_env = malloc(sizeof(char *) * (len_env + 1));
	i = -1;
	if (!tab_env)
		return (NULL);
	while (env)
	{
		tab_env[++i] = ft_strjoin(ft_strjoin(env->name, "="), env->content);
		if (!tab_env[i])
		{
			ft_free_dtab(tab_env);
			return (NULL);
		}
		env = env->next;
	}
	tab_env[len_env] = NULL;
	return (tab_env);
}