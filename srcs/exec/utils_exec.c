/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:09:11 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/02 08:43:40 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_close_save(t_data *data)
{
	if (data->stdin_save != -1)
		close(data->stdin_save);
	if (data->stdout_save != -1)
		close(data->stdout_save);
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
		else if (access(path_cmd, X_OK) == -1)
		{
			*error = 126;
			return (1);
		}
	}
	else
		*error = 127;
	return (0);
}


void	is_cmd_null(t_cmd *cmd, t_data *data)
{
	t_cmd *tmp;
	int i;

	tmp = cmd;
	i = 0;
	while (!tmp->file && tmp->cmd_param && tmp->cmd_param[i]
		&& !*(tmp->cmd_param[i]))
		i++;
	if (!tmp->file && (!tmp->cmd_param || !tmp->cmd_param[i]))
	{
		if (data->fd[0] != -1)
			close(data->fd[0]);
		if (data->fd[1] != -1)
			close(data->fd[1]);
		if (data->prev_fd != -1)
			close(data->prev_fd);
		ft_free_and_exit(*data, NULL);
	}
}