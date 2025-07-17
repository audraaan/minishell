/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:06 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/16 05:58:52 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**parse_path(t_list *env)
{
	char	**path;
	t_list	*tmp;

	// char	*tab;
	// tab = NULL;
	path = NULL;
	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp("PATH", tmp->name))
			break ;
		tmp = tmp->next;
	}
	if (tmp)
	{
		path = ft_split(tmp->content, ':');
		if (!path || !(*path))
		{
			ft_error_msg("malloc parse_path", "failed");
			ft_free_dtab(path);
			return (NULL);
		}
	}
	return (path);
}

char	*search_path(char *cmd, char **path, int *error)
{
	int		i;
	char	*path_cmd;
	char	*path_tmp;

	i = -1;
	while (path && path[++i])
	{
		path_tmp = ft_strjoin(path[i], "/");
		if (!path_tmp)
			return (NULL);
		path_cmd = ft_strjoin(path_tmp, cmd);
		free(path_tmp);
		if (!path_cmd)
			return (NULL);
		if (ft_is_exec(path_cmd, error))
			return (path_cmd);
		free(path_cmd);
	}
	return (NULL);
}

char	*ft_path(char *cmd, t_list *env, int *error)
{
	char	**lst_path;
	char	*path;

	path = NULL;
	if (cmd && !is_builtins(cmd))
	{
		if (!(ft_strchr(cmd, '/') && ft_is_exec(cmd, error)))
		{
			lst_path = parse_path(env);
			if (!lst_path || !*lst_path)
				ft_error("malloc failed parse_path", lst_path, NULL, 1);
			path = search_path(cmd, lst_path, error);
			ft_free_dtab(lst_path);
		}
		else
			path = ft_strdup(cmd);
		if (*error == 127)
			ft_error_msg(cmd, "command not found");
		else if (*error == 126)
		{
			ft_error_msg(cmd, "Permission denied");
			free(path);
		}
	}
	return (path);
}
