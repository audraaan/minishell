/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_msg_and_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:13:44 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/02 04:21:17 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_perror_msg(char *msg, int error)
{
	write(2, "minishell: ", 11);
	perror(msg);
	return (error);
}

int	ft_error_msg(char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (arg && *arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int	ft_error(char *msg, char **path, char **dtab, int status)
{
	if (dtab && dtab[0])
		ft_error_msg(dtab[0], msg);
	else
		ft_error_msg(" ", msg);
	ft_free_dtab(path);
	ft_free_dtab(dtab);
	if (status != -1)
		exit(status);
	return (1);
}

int	ft_free_close_msg(char *msg1, char *msg2, int fd, char *read)
{
	if (msg1 && *msg1 && msg2 && *msg2)
		ft_error_msg(msg1, msg2);
	if (fd != -1)
		close(fd);
	if (read)
		free(read);
	return (0);
}

void	ft_free_and_exit(t_data data, char *path_cmd)
{
	int	status;

	status = data.exit_status;
	free_all(&data, path_cmd);
	exit(status);
}
