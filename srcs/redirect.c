/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:43:04 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/01 11:31:16 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	redirect_outfile(char *file)
{
	int	outfile;

	outfile = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile == -1)
	{
		ft_error("error open", NULL, &file, errno);
		return (1);
	}
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		close(outfile);
		ft_error("error dup2 outfile stdout", NULL, &file, errno);
		return (1);
	}
	close(outfile);
	return (0);
}

int	redirect_outfile_append(char *file)
{
	int	outfile;

	outfile = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (outfile == -1)
	{
		ft_error("error open", NULL, &file, errno);
		return (1);
	}
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		close(outfile);
		ft_error("error dup2 outfile stdout", NULL, &file, errno);
		return (1);
	}
	close(outfile);
	return (0);
}

int	redirect_infile(char *file)
{
	int	infile;

	if (access(file, F_OK) != 0)
	{
		ft_printf("No such file or directory", NULL, &file, 127);
	}
	else if (access(file, R_OK) != 0)
		ft_error("Permission denied", NULL, &file, 1);
	else
	{
		infile = open(file, O_RDONLY);
		if (infile == -1)
		{
			ft_error("error open", NULL, &file, errno);
		}
		if (dup2(infile, STDIN_FILENO) == -1)
		{
			close(infile);
			ft_error("error dup2 infile stdin", NULL, &file, errno);
		}
		close(infile);
	}
	return (0);
}
