/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:43:04 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/06 19:24:56 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir(t_data *data, t_cmd *cmd)
{
	t_file	*tmp;
	int		status;

	status = 0;
	tmp = cmd->file;
	return (ft_loop_redir(data, tmp, status));
}

int	ft_loop_redir(t_data *data, t_file *tmp, int status)
{
	while (tmp)
	{
		if (tmp->type == REDIR_IN || tmp->type == HEREDOC)
		{
			status = redirect_infile(tmp->filename);
			if (status != 0)
				return (status);
		}
		else if (tmp->type == REDIR_OUT)
		{
			status = redirect_outfile(tmp->filename);
			if (status != 0)
				return (status);
		}
		else if (tmp->type == APPEND)
		{
			status = redirect_outfile_append(tmp->filename);
			if (status != 0)
				return (status);
		}
		tmp = tmp->next;
	}
	return (status);
}

int	redirect_outfile(char *file)
{
	int	outfile;

	outfile = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0622);
	if (outfile == -1)
		return (ft_perror_msg(file, 1));
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		close(outfile);
		ft_error("error dup2 outfile stdout", NULL, NULL, -1);
		return (1);
	}
	close(outfile);
	return (0);
}

int	redirect_outfile_append(char *file)
{
	int	outfile;

	outfile = open(file, O_CREAT | O_WRONLY | O_APPEND, 0622);
	if (outfile == -1)
		return (ft_perror_msg(file, 1));
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		close(outfile);
		ft_error("error dup2 outfile stdout", NULL, NULL, -1);
		return (1);
	}
	close(outfile);
	return (0);
}

int	redirect_infile(char *file)
{
	int	infile;

	if (!g_exit_status)
	{
		if (access(file, F_OK) != 0)
			return (ft_error_msg(file, "No such file or directory"));
		else if (access(file, R_OK) != 0)
			return (ft_error_msg(file, "Permission denied"));
		else
		{
			infile = open(file, O_RDONLY);
			if (infile == -1)
				return (ft_perror_msg(file, 1));
			if (dup2(infile, STDIN_FILENO) == -1)
			{
				close(infile);
				ft_error("error dup2 infile stdin\n", NULL, NULL, -1);
			}
			close(infile);
		}
	}
	return (g_exit_status);
}
