/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:38:22 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/20 05:44:45 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_child_builtins(t_cmd *cmd, t_data *data)
{
	if (data->prev_fd != -1)
	{
		dup2(data->prev_fd, STDIN_FILENO);
		close(data->prev_fd);
	}
	if (cmd->next)
	{
		close(data->fd[0]);
		dup2(data->fd[1], STDOUT_FILENO);
		close(data->fd[1]);
	}
	if (handle_redir(cmd))
		exit(1);
	if (!cmd->cmd_param[0])
		exit(0);
	return (builtins(cmd->cmd_param, &data->env));
}

int	is_builtins(t_cmd *cmd)
{
	if (cmd->cmd_param[0] && (!ft_strcmp(cmd->cmd_param[0], "env")
			|| !ft_strcmp(cmd->cmd_param[0], "export")
			|| !ft_strcmp(cmd->cmd_param[0], "unset")
			|| !ft_strcmp(cmd->cmd_param[0], "cd")
			|| !ft_strcmp(cmd->cmd_param[0], "pwd")
			|| !ft_strcmp(cmd->cmd_param[0], "echo")
			|| !ft_strcmp(cmd->cmd_param[0], "exit")))
		return (1);
	return (0);
}

int	builtins(char **cmd, t_list **env)
{
	if (!ft_strcmp(cmd[0], "env"))
		ft_env(*env);
	else if (!ft_strcmp(cmd[0], "unset"))
		ft_unset(env, cmd);
	else if (!ft_strcmp(cmd[0], "export"))
		ft_export(env, cmd);
	else if (!ft_strcmp(cmd[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd[0], "cd"))
		ft_cd(env, cmd);
	else
		return (1);
	return (0);
}
