/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:06 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/20 05:30:50 by alarroye         ###   ########lyon.fr   */
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
		if (ft_is_exec(path_cmd, error))
			return (path_cmd);
		free(path_cmd);
	}
	return (NULL);
}
void	ft_status_path(char *cmd, int *error, char *path)
{
	if (*error == 127)
		ft_error_msg(cmd, "command not found");
	else if (*error == 126)
	{
		ft_error_msg(cmd, "Permission denied");
		free(path);
	}
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
		if (!(ft_strchr(cmd_tab, '/') && ft_is_exec(cmd_tab, error)))
		{
			lst_path = parse_path(env);
			if (!lst_path || !*lst_path)
				ft_error("malloc failed parse_path", lst_path, NULL, 1);
			path = search_path(cmd_tab, lst_path, error);
			ft_free_dtab(lst_path);
		}
		else
			path = ft_strdup(cmd_tab);
		ft_status_path(cmd_tab, error,path);
	}
	return (path);
}
