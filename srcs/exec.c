/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:24:14 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/20 09:56:30 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exec(t_data *data, pid_t pid)
{
	t_cmd	*cmd;
	char	*path_cmd;

	cmd = data->cmd;
	while (cmd)
	{
		if (cmd->next && pipe(data->fd) == -1)
			return (ft_error_msg("pipe", "cannot create pipe"));
		path_cmd = ft_path(cmd, data->env, &data->error);
		if (data->error == 127 || data->error == 126)
		{
			cmd = cmd->next;
			continue ;
		}
		if (ft_cmdlen(data->cmd) == 1 && cmd->cmd_param[0] && is_builtins(cmd)
			&& !handle_redir(cmd) && !builtins(cmd->cmd_param, &data->env))
			break ;
		else
			pid = handle_children(pid, cmd, data, path_cmd);
		if (path_cmd && *path_cmd)
			free(path_cmd);
		cmd = cmd->next;
	}
	data->prev_fd = -1;
	return (ft_wait(data->cmd, pid, &data->error));
}

pid_t	handle_children(pid_t pid, t_cmd *cmd, t_data *data, char *path_cmd)
{
	pid = fork();
	if (pid == -1)
		return (ft_error_msg("fork", "cannot fork"));
	if (pid == 0)
	{
		ft_close_save(data);
		if (cmd->cmd_param[0] && is_builtins(cmd))
		{
			ft_child_builtins(cmd, data);
			free_all(*data, NULL);
			close(data->fd[0]);
			exit(data->exit_status);
		}
		ft_child(cmd, path_cmd, data);
	}
	if (data->prev_fd != -1)
		close(data->prev_fd);
	if (cmd->next)
	{
		close(data->fd[1]);
		data->prev_fd = data->fd[0];
	}
	else if (data->fd[0] != -1)
		close(data->fd[0]);
	return (pid);
}

int	ft_child(t_cmd *cmd, char *path_cmd, t_data *data)
{
	char	**env_exec;

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
	{
		free_all(*data, path_cmd);
		exit(data->exit_status);
	}
	if (!cmd->cmd_param[0])
	{
		free_all(*data, path_cmd);
		exit(data->exit_status);
	}
	env_exec = lst_in_tab(data->env);
	if (!env_exec)
		return (ft_error_msg("lst_in_tab:", "malloc failed"));
	execve(path_cmd, cmd->cmd_param, env_exec);
	return (ft_failed_execve(data, cmd->cmd_param, env_exec, path_cmd));
}

int	ft_failed_execve(t_data *data, char **cmd, char **env, char *path_cmd)
{
	free_all(*data, NULL);
	ft_free_dtab(env);
	if (path_cmd)
		free(path_cmd);
	perror("execve");
	exit(errno);
	return (0);
}

int	ft_wait(t_cmd *head, pid_t pid, int *error)
{
	while (head)
	{
		waitpid(-1, error, 0);
		head = head->next;
	}
	return (0);
}
