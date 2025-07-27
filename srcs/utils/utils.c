/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:09 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/27 07:05:44 by alarroye         ###   ########lyon.fr   */
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

void	ft_close_save(t_data *data)
{
	if (data->stdin_save != -1)
		close(data->stdin_save);
	if (data->stdout_save != -1)
		close(data->stdout_save);
}

int	ft_perror_msg(char *msg, int error)
{
	write(2, "minishell: ", 11);
	perror(msg);
	return (error);
}

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

void	ft_free_and_exit(t_data data, char *path_cmd)
{
	int	status;

	status = data.exit_status;
	free_all(&data, path_cmd);
	exit(status);
}

char	**ft_free_and_null(char **tab)
{
	ft_free_dtab(tab);
	return (NULL);
}

char	**lst_in_tab(t_list *env)
{
	int		i;
	char	**tab_env;
	char	*tmp;

	tab_env = ft_calloc(sizeof(char *), (ft_lstlen(env) + 1));
	if (!tab_env)
		return (NULL);
	i = -1;
	while (env)
	{
		if (env->content)
		{
			tmp = ft_strjoin(env->name, "=");
			if (!tmp)
				return (ft_free_and_null(tab_env));
			tab_env[++i] = ft_strjoin(tmp, env->content);
			free(tmp);
			if (!tab_env[i])
				return (ft_free_and_null(tab_env));
		}
		env = env->next;
	}
	tab_env[++i] = NULL;
	return (tab_env);
}


int	ft_free_close_msg(char *msg1, char *msg2, int fd, char *read)
{
	if (msg1 && *msg1 && msg2 && *msg2)
		ft_error_msg(msg1, msg2);
	if (fd != -1)
		close(fd);
	if (read)
		free(read);
	return (0);
}

// t_list	*cpy_env(char **env)
//{
//	int		i;
//	int		j;
//	char	*name;
//	char	*content;
//	t_list	*env_cpy;

//	env_cpy = NULL;
//	i = 0;
//	j = 0;
//	while (env[i])
//	{
//		j = 0;
//		while (env[i][j] && env[i][j] != '=')
//			j++;
//		name = ft_substr(env[i], 0, j);
//		if (env[i][j] == '=')
//			content = ft_strdup(&env[i][j + 1]);
//		else
//			content = NULL;
//		ft_lstadd_back(&env_cpy, ft_lstnew(name, content));
//		i++;
//	}
//	return (env_cpy);
//}
