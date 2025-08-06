/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:06 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/06 19:08:56 by alarroye         ###   ########lyon.fr   */
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
	while (path && path[++i] && cmd && *cmd)
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

char	*ft_path(t_cmd *cmd, t_list *env, int *error)
{
	char	**lst_path;
	char	*path;
	char	*cmd_tab;

	path = NULL;
	cmd_tab = cmd->cmd_param[0];
	if (cmd_tab && !is_builtins(cmd))
	{
		if (ft_strchr(cmd_tab, '/'))
			path = ft_absolute_path(cmd_tab, error);
		else
		{
			lst_path = parse_path(env);
			path = search_path(cmd_tab, lst_path, error);
			ft_free_dtab(lst_path);
			if (ft_status_path(cmd_tab, error, path))
			{
				if (path && *path)
					free(path);
				return (NULL);
			}
		}
	}
	return (path);
}

int	ft_status_path(char *cmd, int *error, char *path)
{
	DIR	*directory;

	if (*error == 0)
	{
		directory = opendir(cmd);
		if (directory)
		{
			closedir(directory);
			*error = 126;
			return (1);
		}
	}
	if (*error == 126)
	{
		return (1);
	}
	else if (*error == 127 || !path)
	{
		*error = 127;
		return (1);
	}
	return (0);
}

char	*ft_absolute_path(char *cmd, int *error)
{
	DIR	*directory;

	if (ft_is_exec(cmd, error))
	{
		if (*error == 0)
		{
			directory = opendir(cmd);
			if (directory)
			{
				*error = 126;
				closedir(directory);
				return (NULL);
			}
			else
				return (ft_strdup(cmd));
		}
	}
	return (NULL);
}
