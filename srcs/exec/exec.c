/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:24:14 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/06 20:38:37 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exec(t_data *data, pid_t pid)
{
	t_cmd	*cmd;
	char	*path_cmd;

	path_cmd = NULL;
	cmd = data->cmd;
	data->exit_status = 0;
	while (cmd)
	{
		if (cmd->next && pipe(data->fd) == -1)
			return (ft_error_msg("pipe", "cannot create pipe"));
		if (is_sigle_builtins(data, cmd))
			break ;
		else
		{
			pid = fork();
			if (pid == -1)
				return (ft_error_msg("fork", "cannot fork"));
			handle_children(&pid, cmd, data, path_cmd);
		}
		cmd = cmd->next;
	}
	data->prev_fd = -1;
	return (ft_wait(data, pid));
}

void	handle_children(pid_t *pid, t_cmd *cmd, t_data *data, char *path_cmd)
{
	if (*pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ft_close_save(data);
		is_cmd_null(cmd, data);
		path_cmd = ft_path(cmd, data->env, &data->exit_status);
		if (cmd->cmd_param[0] && is_builtins(cmd))
		{
			ft_child_builtins(cmd, data);
			ft_free_and_exit(*data, path_cmd);
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
}

int	ft_child(t_cmd *cmd, char *path_cmd, t_data *data)
{
	char	**env_exec;
	t_list	*tmp_env;

	tmp_env = data->env;
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
	check_cmd(data, cmd, path_cmd);
	env_exec = lst_in_tab(tmp_env);
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
}

int	ft_wait(t_data *data, pid_t pid)
{
	int	status;
	int	w_pid;
	int	err;
	int	len_cmd;

	len_cmd = ft_cmdlen(data->cmd);
	if (len_cmd == 1 && is_builtins(data->cmd))
		return (data->exit_status);
	err = 0;
	w_pid = 0;
	status = 0;
	while (len_cmd--)
	{
		w_pid = waitpid(-1, &status, 0);
		if (w_pid == pid && WIFEXITED(status))
			err = WEXITSTATUS(status);
		else if (w_pid == pid && WIFSIGNALED(status))
			err = (128 + WTERMSIG(status));
	}
	if (err == 131 && data->stdout_save != -1)
		write(data->stdout_save, "Quit (core dumped)", 18);
	return (err);
}
