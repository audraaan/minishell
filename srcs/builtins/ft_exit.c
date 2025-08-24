/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:12:33 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/30 22:48:19 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clean_all(t_data *data, int tmp_exit_code)
{
	int	exit_code;

	exit_code = tmp_exit_code;
	ft_close_save(data);
	free_all(data, NULL);
	exit(exit_code);
}

int	ft_exit(t_data *data, t_cmd *cmd)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		write(1, "exit\n", 5);
	if (cmd->file)
		write(data->stdout_save, "exit\n", 5);
	if (cmd->cmd_param && !cmd->cmd_param[1])
		ft_clean_all(data, data->exit_status);
	if (!ft_str_isdigit(cmd->cmd_param[1]))
	{
		ft_error_msg("exit", "numeric argument required");
		ft_clean_all(data, 2);
	}
	else if (cmd->cmd_param[2])
		return (ft_error_msg("exit", "too many arguments"));
	else
		ft_clean_all(data, (unsigned char)ft_atoi(cmd->cmd_param[1]));
	return (0);
}

void	ft_ctrl_d(t_data *data, char *read)
{
	int	save_status;

	save_status = data->exit_status;
	if (isatty(STDIN_FILENO))
		write(data->stdout_save, "exit\n", 5);
	ft_close_save(data);
	free_all(data, read);
	exit(save_status);
}
