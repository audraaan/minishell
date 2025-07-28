/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:06 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/28 04:07:00 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_path(t_list *env)
{
	char	**path;
	t_list	*tmp;

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
		if (ft_is_exec(path_cmd, error) && (*error != 126))
			return (path_cmd);
		free(path_cmd);
		if (*error == 126)
			return (NULL);
	}
	return (NULL);
}

int	ft_status_path(char *cmd, int *error, char *path)
{
	if (*error == 126)
	{
		if (path && *path)
			free(path);
		return (ft_error_msg(cmd, "Permission denied"));
	}
	else if (*error == 127 || !path)
	{
		*error = 127;
		return (ft_error_msg(cmd, "command not found"));
	}
	return (0);
}

char	*ft_path(t_cmd *cmd, t_list *env, int *error)
{
	char	**lst_path;
	char	*path;
	char	*cmd_tab;

	path = NULL;
	cmd_tab = cmd->cmd_param[0];
	if (cmd_tab && !is_builtins(cmd))
	{
		if (ft_strchr(cmd_tab, '/') && ft_is_exec(cmd_tab, error))
		{
			path = ft_strdup(cmd_tab);
		}
		else
		{
			lst_path = parse_path(env);
			path = search_path(cmd_tab, lst_path, error);
			ft_free_dtab(lst_path);
		}
		if (ft_status_path(cmd_tab, error, path))
			return (NULL);
	}
	return (path);
}
