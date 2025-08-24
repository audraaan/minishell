/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:56:22 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/06 20:38:45 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_cmd(t_data *data, t_cmd *cmd, char *path)
{
	int	redir_status;

	redir_status = handle_redir(data, cmd);
	if (redir_status)
	{
		data->exit_status = redir_status;
		ft_free_and_exit(*data, path);
	}
	if (!cmd->cmd_param[0])
		ft_free_and_exit(*data, path);
	if (!path)
		check_status(data, cmd->cmd_param[0], path);
}

int	is_sigle_builtins(t_data *data, t_cmd *cmd)
{
	int	status_redir;

	status_redir = 0;
	if (ft_cmdlen(data->cmd) == 1 && cmd->cmd_param[0] && is_builtins(cmd))
	{
		status_redir = handle_redir(data, cmd);
		if (!status_redir && !builtins(cmd, data))
			return (1);
		else if (status_redir)
		{
			data->exit_status = status_redir;
			return (1);
		}
	}
	return (0);
}

void	check_status(t_data *data, char *cmd, char *path)
{
	DIR	*directory;

	if (data->exit_status == 127)
	{
		ft_error_msg(cmd, "command not found");
		ft_free_and_exit(*data, path);
	}
	else if (data->exit_status == 126)
	{
		directory = opendir(cmd);
		if (directory)
		{
			closedir(directory);
			ft_error_msg(cmd, "Is a directory");
			ft_free_and_exit(*data, path);
		}
		else
		{
			ft_error_msg(cmd, "Permission denied");
			ft_free_and_exit(*data, path);
		}
	}
}

void	param_is_double_quotes(char **cmd_param)
{
	char	**tmp;
	int		i;
	int		j;
	char	*param;

	i = 0;
	j = -1;
	tmp = cmd_param;
	while (tmp[1] && tmp[++i])
	{
		while (tmp[i][++j])
			if (tmp[i][j] != '\"')
				break ;
		if (!tmp[i][j])
		{
			if (cmd_param && cmd_param[i])
			{
				param = cmd_param[i];
				cmd_param[i] = ft_strdup("");
				free(param);
			}
		}
		j = -1;
	}
}

void	param_is_quotes(char **cmd_param)
{
	char	**tmp;
	int		i;
	int		j;
	char	*param;

	i = 0;
	j = -1;
	tmp = cmd_param;
	while (tmp[1] && tmp[++i])
	{
		while (tmp[i][++j])
			if (tmp[i][j] != '\'')
				break ;
		if (!tmp[i][j])
		{
			if (cmd_param && *cmd_param && cmd_param[i])
			{
				param = cmd_param[i];
				cmd_param[i] = ft_strdup("");
				free(param);
			}
		}
		j = -1;
	}
	param_is_double_quotes(cmd_param);
}
