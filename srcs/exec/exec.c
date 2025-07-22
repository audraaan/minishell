/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:24:14 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/22 06:26:56 by alarroye         ###   ########lyon.fr   */
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
		data->exit_status = 0;
		if (cmd->next && pipe(data->fd) == -1)
			return (ft_error_msg("pipe", "cannot create pipe"));
		path_cmd = ft_path(cmd, data->env, &data->exit_status);
		if (data->exit_status == 127 || data->exit_status == 126)
		{
			cmd = cmd->next;
			continue ;
		}
		if (ft_cmdlen(data->cmd) == 1 && cmd->cmd_param[0] && is_builtins(cmd)
			&& !handle_redir(data, cmd) && !builtins(cmd->cmd_param, data))
			break ;
		else
			pid = handle_children(pid, cmd, data, path_cmd);
		cmd = cmd->next;
	}
	data->prev_fd = -1;
	return (ft_wait(data->cmd, pid, &data->exit_status));
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
			ft_free_and_exit(*data, NULL);
		}
		ft_child(cmd, path_cmd, data);
	}
	if (path_cmd && *path_cmd)
		free(path_cmd);
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
	if (handle_redir(data, cmd))
	{
		printf("EXIT=%i", data->exit_status);
		ft_free_and_exit(*data, path_cmd);
	}
	if (!cmd->cmd_param[0])
		ft_free_and_exit(*data, path_cmd);
	env_exec = lst_in_tab(data->env);
	if (!env_exec)
		return (ft_error_msg("lst_in_tab:", "malloc failed"));
	execve(path_cmd, cmd->cmd_param, env_exec);
	return (ft_failed_execve(data, cmd->cmd_param, env_exec, path_cmd));
}

int	ft_failed_execve(t_data *data, char **cmd, char **env, char *path_cmd)
{
	free_all(data, path_cmd);
	ft_free_dtab(env);
	perror("failed execve");
	exit(errno);
	return (0);
}

int	ft_wait(t_cmd *head, pid_t pid, int *status)
{
	while (head)
	{
		waitpid(-1, status, 0);
		head = head->next;
	}
	if (!WIFEXITED(*status))
		return (128 + WTERMSIG(*status));
	return (WEXITSTATUS(*status));
}
