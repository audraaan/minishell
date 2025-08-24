/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 06:21:39 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/04 00:01:06 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_read_urandom(char *name, int fd)
{
	int		nb_bytes;
	int		i;
	char	buf[2];

	i = 0;
	buf[1] = '\0';
	while (i < 20)
	{
		nb_bytes = read(fd, buf, 1);
		if (nb_bytes != 1)
		{
			free(name);
			perror("read: urandom");
			return (1);
		}
		if (ft_isalpha(buf[0]))
		{
			name[i] = buf[0];
			i++;
		}
	}
	return (0);
}

char	*ft_open_urandom(void)
{
	int		fd;
	char	*name;

	name = malloc(sizeof(char) * 21);
	if (!name)
		return (ft_error_msg(NULL, "malloc failed"), NULL);
	name[20] = '\0';
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		free(name);
		perror("open: urandom");
		return (NULL);
	}
	if (ft_read_urandom(name, fd))
		return (NULL);
	close(fd);
	return (name);
}

int	ft_tmp_file(t_file **file)
{
	int		fd;
	char	*name;
	char	*name_tmp;

	name_tmp = ft_open_urandom();
	if (!name_tmp)
		return (-1);
	name = ft_strjoin("/tmp/", name_tmp);
	free(name_tmp);
	if (!name)
	{
		free(name);
		ft_error_msg("ft_strjoin", "malloc failed");
		return (-1);
	}
	fd = open(name, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
	{
		perror("open:");
		return (-1);
	}
	(*file)->filename = name;
	return (fd);
}

int	handle_heredoc(t_data *data)
{
	t_cmd	*tmp;
	t_file	*tmp_file;
	int		fd;

	tmp = data->cmd;
	while (tmp)
	{
		tmp_file = tmp->file;
		while (tmp_file)
		{
			if (tmp_file->type == HEREDOC)
			{
				fd = ft_tmp_file(&tmp_file);
				if (fd == -1)
					return (1);
				ft_heredoc(tmp_file, fd);
			}
			tmp_file = tmp_file->next;
		}
		tmp = tmp->next;
	}
	return (g_exit_status);
}

int	ft_heredoc(t_file *tmp, int fd)
{
	char	*read;

	while (!g_exit_status)
	{
		//read = readline("> ");
		if (isatty(fileno(stdin)))
			read = readline("> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			read = ft_strtrim(line, "\n");
			free(line);
		}
		if (!read)
			return (ft_free_close_msg("warning",
					"here-document delimited by end-of-file", fd, NULL));
		if (ft_strcmp(read, tmp->eof))
		{
			write(fd, read, ft_strlen(read));
			write(fd, "\n", 1);
		}
		else
		{
			ft_free_close_msg(NULL, NULL, fd, read);
			break ;
		}
		if (!read[0])
		{
			write(fd, "\n", 1);
			continue ;
		}
	}
	return (close(fd));
}
