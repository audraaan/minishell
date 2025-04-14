/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/14 13:53:06 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_free_dtab(char **tab)
{
	int	i;

	i = -1;
	while (tab && tab[++i])
		free(tab[i]);
	if (tab)
		free(tab);
	return (1);
}

int	ft_is_exec(char *path_cmd, int *error)
{
	if (access(path_cmd, F_OK) == 0)
	{
		if (access(path_cmd, X_OK) == 0)
		{
			*error = 0;
			return (1);
		}
		else
		{
			*error = 126;
			return (1);
		}
	}
	else
		*error = 127;
	return (0);
}

char	**parse_env(char **env)
{
	char	*tab;
	char	**path;
	int		i;

	tab = NULL;
	i = 0;
	path = NULL;
	while (env[i])
	{
		if (!ft_strncmp("PATH=", env[i], 5))
			break ;
		i++;
	}
	if (env[i])
		tab = ft_strdup(&env[i][4]);
	if (!tab)
		return (NULL);
	path = ft_split(tab, ':');
	free(tab);
	if (!path || !(*path))
		ft_free_dtab(path);
	return (path);
}

char	*search_path(char *cmd, char **path, int *error)
{
	int		i;
	char	*path_cmd;
	char	*path_tmp;

	i = -1;
	while (path[++i])
	{
		path_tmp = ft_strjoin(path[i], "/");
		if (!path_tmp)
			return (NULL);
		path_cmd = ft_strjoin(path_tmp, cmd);
		free(path_tmp);
		if (!path_cmd)
			return (NULL);
		if (ft_is_exec(path_cmd, error))
			return (path_cmd);
		free(path_cmd);
	}
	return (NULL);
}

void	ft_error_msg(char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

int	redirect_input(char *file, char **path)
{
	int	infile;

	if (access(file, F_OK) != 0)
		ft_error("No such file or directory", path, &file, 127);
	else if (access(file, R_OK) != 0)
		ft_error("Permission denied", path, &file, 1);
	else
	{
		infile = open(file, O_RDONLY);
		if (infile == -1)
			ft_error("error open", path, &file, errno);
		if (dup2(infile, STDIN_FILENO) == -1)
		{
			close(infile);
			ft_error("error dup2 infile stdin", path, &file, errno);
		}
		close(infile);
	}
	return (0);
}

int	ft_error(char *msg, char **path, char **dtab, int status)
{
	if (dtab)
		ft_error_msg(dtab[0], msg);
	else
		ft_error_msg(dtab[0], msg);
	ft_free_dtab(path);
	ft_free_dtab(dtab);
	if (status != -1)
		exit(status);
	return (1);
}

int	main(int ac, char **av, char **env)
{
	char	**cmd;
	char	**path;
	char	*path_cmd;
	char	*read;
	int		status;
	pid_t	pid;
	//char	*file;

	(void)ac;
	(void)av;
	while (1)
	{
		path = parse_env(env);
		read = readline("minishell> ");
		if (!read || (ft_strcmp(read, "exit") == 0))
			break ;
		if (!read[0])
			continue ;
		// printf("read=%s;\n",read);
		if (ft_strchr(read, '<'))
			read = &(ft_strchr(read, '<')[1]);
		cmd = ft_split(read, ' ');
		free(read);
		if (!cmd || !cmd[0])
			return (ft_error("split error", path, cmd, -1));
		path_cmd = search_path(cmd[0], path, &status);
		if (!path_cmd)
		{
			ft_error("command not found", path, cmd, -1);
		}
		else if (status == 126)
			ft_error("Permission denied", path, cmd, -1);
		else
		{
			pid = fork();
			if (pid == -1)
				return (ft_error("pid error", path, cmd, errno));
			else if (pid == 0)
				execve(path_cmd, cmd, env);
			else
				waitpid(pid, &status, 0);
		}
	}
}
