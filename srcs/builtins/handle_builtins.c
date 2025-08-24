/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:38:22 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/02 21:27:30 by alarroye         ###   ########lyon.fr   */
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
	if (handle_redir(data, cmd))
		exit(1);
	if (!cmd->cmd_param[0])
		exit(0);
	close(data->fd[0]);
	return (builtins(cmd, data));
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

int	builtins(t_cmd *cmd, t_data *data)
{
	if (!ft_strcmp(cmd->cmd_param[0], "env"))
		data->exit_status = ft_env(data->env, cmd->cmd_param);
	else if (!ft_strcmp(cmd->cmd_param[0], "unset"))
		data->exit_status = ft_unset(&(data->env), cmd->cmd_param);
	else if (!ft_strcmp(cmd->cmd_param[0], "export"))
		data->exit_status = ft_export(&(data->env), cmd->cmd_param, data);
	else if (!ft_strcmp(cmd->cmd_param[0], "pwd"))
		data->exit_status = ft_pwd();
	else if (!ft_strcmp(cmd->cmd_param[0], "cd"))
		data->exit_status = ft_cd(&(data->env), cmd->cmd_param, data);
	else if (!ft_strcmp(cmd->cmd_param[0], "exit"))
		data->exit_status = ft_exit(data, cmd);
	else if (!ft_strcmp(cmd->cmd_param[0], "echo"))
		data->exit_status = ft_echo(cmd->cmd_param);
	else
		return (1);
	return (0);
}
