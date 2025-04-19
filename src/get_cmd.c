/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:39:06 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/19 00:42:00 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**parse_path(char **env)
{
	char	*tab;
	char	**path;
	int		i;

	tab = NULL;
	i = 0;
	path = NULL;
	while (env[i])
	{
		if (!ft_strncmp("PATH=", env[i], 5))
			break ;
		i++;
	}
	if (env[i])
		tab = ft_strdup(&env[i][4]);
	if (!tab)
		return (NULL);
	path = ft_split(tab, ':');
	free(tab);
	if (!path || !(*path))
		ft_free_dtab(path);
	return (path);
}

char	*search_path(char *cmd, char **path, int *error)
{
	int		i;
	char	*path_cmd;
	char	*path_tmp;

	i = -1;
	while (path[++i])
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