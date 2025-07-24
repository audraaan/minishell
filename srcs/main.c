/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/24 13:48:12 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 1;
}

void	sigquit_handler(int sig)
{
	(void)sig;
	printf("Quit (core dumped)\n");
	g_exit_status = 131;
}

void	init_data(t_data *data, int ac, char **av)
{
	(void)ac;
	(void)av;
	ft_bzero(data, sizeof(t_data));
	data->stdout_save = -1;
	data->stdin_save = -1;
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->prev_fd = -1;
	if (!data->stdin_save || !data->stdout_save)
	{
		printf("save dup failed\n");
		exit(1);
	}
	g_exit_status = 0;
}

int	er_msg_free_tok(char *arg, char *msg, t_token **token)
{
	char	*tmp;
	int		res;

	tmp = NULL;
	if (arg)
		tmp = ft_strdup(arg);
	if (token)
	{
		free_tokens(token);
		token = NULL;
	}
	res = ft_error_msg(tmp, msg);
	if (tmp && *tmp)
		free(tmp);
	return (res);
}

int	check_synthax(t_data *data)
{
	t_token	*token;

	token = data->token;
	if (!(token->str))
		return (er_msg_free_tok(NULL, "command not found", &data->token));
	if (token->type == PIPE)
		return (er_msg_free_tok(token->str,
				"syntax error near unexpected token", &data->token));
	while (token)
	{
		if ((!token->str || token->str[0] == '\0'))
			return (er_msg_free_tok(token->str, "command not found",
					&data->token));
		if (token->type == PIPE && (!token->next || token->next->type == PIPE))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		else if (!(token->type == PIPE || token->type == WORD) && (!token->next
				|| token->next->type != WORD))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		token = token->next;
	}
	return (0);
}

char	*ft_read_urandom(void)
{
	int		fd;
	int		nb_bytes;
	char	buf[2];
	char	*name;
	int		i;

	name = malloc(sizeof(char) * 21);
	if (!name)
		return (ft_error_msg(NULL, "malloc failed"), NULL);
	name[20] = '\0';
	buf[1] = '\0';
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		free(name);
		perror("open: urandom");
		return (NULL);
	}
	i = 0;
	while (i < 20)
	{
		nb_bytes = read(fd, buf, 1);
		if (nb_bytes != 1)
		{
			free(name);
			perror("read: urandom");
			return (NULL);
		}
		if (ft_isalpha(buf[0]))
		{
			name[i] = buf[0];
			i++;
		}
	}
	close(fd);
	return (name);
}

int	ft_tmp_file(t_file **file)
{
	int		fd;
	char	*name;
	char	*name_tmp;

	name_tmp = ft_read_urandom();
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

void	handle_heredoc(t_data *data)
{
	t_cmd	*tmp;
	t_file	*tmp_file;

	tmp = data->cmd;
	while (tmp)
	{
		tmp_file = tmp->file;
		while (tmp_file)
		{
			// printf("tmpfilename=%s\n", tmp_file->filename);
			// printf("tmpfileeof=%s\n", tmp_file->eof);
			if (tmp_file->type == HEREDOC)
				ft_heredoc(tmp_file);
			tmp_file = tmp_file->next;
			// printf("cmd=%s\n", data->cmd->file->filename);
			// printf("cmdfileeof=%s\n", data->cmd->file->eof);
		}
		tmp = tmp->next;
	}
}

void	ft_heredoc(t_file *tmp)
{
	char	*read;
	int		fd;

	fd = -1;
	fd = ft_tmp_file(&tmp);
	if (fd == -1)
		return ;
	while (1)
	{
		// printf("%s", tmp->filename);
		read = readline("> ");
		if (!read)
		{
			ft_error_msg("warning", "here-document delimited by end-of-file");
			close(fd);
			return ;
		}
		if (ft_strcmp(read, tmp->eof))
		{
			write(fd, read, ft_strlen(read));
			write(fd, "\n", 1);
		}
		else
		{
			free(read);
			close(fd);
			break ;
		}
		if (!read[0])
		{
			write(fd, "\n", 1);
			continue ;
		}
	}
	close(fd);
}
// int	handle_shlvl(char ***arg, char *content)
//{
//	*arg = malloc(sizeof(char *) * 3);
//	if (!(*arg))
//		return (ft_error_msg("handle_shlvl", "malloc failed"));
//	*arg[0] = ft_strdup("export");
//	if (!(*arg[0]))
//		return (ft_error("handle_shlvl: malloc failed", *arg, NULL, -1));
//	*arg[1] = content;
//	*arg[2] = NULL;
//	return (0);
//}

// int	update_shlvl(t_data *data)
//{
//	t_list	*tmp_env;
//	int		res;
//	char	**arg;
//	char	*content;

//	tmp_env = data->env;
//	while (ft_strcmp(tmp_env->name, "SHLVL"))
//		tmp_env = tmp_env->next;
//	if (!ft_strcmp(tmp_env->name, "SHLVL"))
//	{
//		res = ft_atoi(tmp_env->content);
//		res++;
//		content = ft_itoa(res);
//		if (!content)
//			return (ft_error_msg("update_shlvl: ft_itoa", "malloc failed"));
//		if (handle_shlvl(&arg, content))
//			return (1);
//		return (ft_export(&(data->env), arg, data));
//	}
//	if (handle_shlvl(&arg, "0"))
//		return (1);
//	return (ft_export(&(data->env), arg, data));
//}
int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*read;
	pid_t	pid;

	init_data(&data, ac, av);
	pid = 0;
	data.env = cpy_env(env);
	// if (update_shlvl(&data))
	//	return (1);
	data.stdin_save = dup(STDIN_FILENO);
	data.stdout_save = dup(STDOUT_FILENO);
	if (data.stdin_save == -1 || data.stdout_save == -1)
		return (ft_error_msg("dup", "dup failed"));
	if (!data.env)
		return (ft_error_msg("cpy_env", "Error: Failed to copy environment"));
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		dup2(data.stdin_save, STDIN_FILENO);
		dup2(data.stdout_save, STDOUT_FILENO);
		read = readline("minishell> ");
		if (g_exit_status)
			data.exit_status = 130;
		g_exit_status = 0;
		if (!read)
		{
			ft_close_save(&data);
			break ;
		}
		if (!read[0])
		{
			free(read);
			continue ;
		}
		add_history(read);
		data.token = tokenize(&data, read);
		free(read);
		if (!data.token || check_synthax(&data))
		{
			free_tokens(&data.token);
			continue ;
		}
		expand_tokens(&data);
		data = cmd_builder(&data);
		//print_list(data.env);
		// print_tokens(data.token);
		// print(data.cmd);
		handle_heredoc(&data);
		data.exit_status = ft_exec(&data, pid);
		free_iteration_data(&data);
	}
	rl_clear_history();
	free_all(&data, read);
	return (0);
}

// t_cmd	*handle_cmd(char *str)
//{
//	int		len;
//	char	**cmd_tab;
//	char	*tmp_str;
//	char	*new_str;
//	t_cmd	*tmp_cmd;
//	t_cmd	*cmd;

//	if (!str || !(*str))
//		return (NULL);
//	cmd = (t_cmd *)malloc(sizeof(t_cmd));
//	tmp_cmd = (t_cmd *)malloc(sizeof(t_cmd));
//	cmd->next = tmp_cmd;
//	new_str = ft_strdup(str);
//	while (new_str)
//	{
//		if (!tmp_cmd)
//			tmp_cmd = (t_cmd *)malloc(sizeof(t_cmd));
//		tmp_cmd->next = NULL;
//		if (ft_strchr(new_str, '|'))
//			len = ft_strlen(new_str) - ft_strlen(ft_strchr(new_str, '|'));
//		else
//			len = ft_strlen(new_str);
//		tmp_str = ft_strndup(new_str, len);
//		cmd_tab = ft_split(tmp_str, ' ');
//		free(tmp_str);
//		tmp_cmd->cmd_param = cmd_tab;
//		if (*new_str && ft_strchr(new_str, '|') && &ft_strchr(new_str, '|')[1]
//			&& *(&ft_strchr(new_str, '|')[1]))
//		{
//			tmp_str = new_str;
//			new_str = ft_strdup(&ft_strchr(new_str, '|')[1]);
//			free(tmp_str);
//		}
//		else
//			new_str = NULL;
//		tmp_cmd = tmp_cmd->next;
//	}
//	return (cmd->next);
//}

// int	ft_exec(t_data *data, pid_t pid)
//{
//	t_cmd	*cmd;
//	char	*path_cmd;
//	int		error;
//	int		fd[2];
//	int		prev_fd;
//	t_cmd	*head;

//	cmd = data->cmd;
//	head = data->cmd;
//	prev_fd = -1;
//	error = 0;
//	while (cmd)
//	{
//		if (cmd->next && pipe(fd) == -1)
//			return (ft_printf("pipe error\n"), 1);
//		path_cmd = ft_path(cmd->cmd_param[0], data->env, &error);
//		if (error == 127 || error == 126)
//		{
//			cmd = cmd->next;
//			continue ;
//		}
//		if (ft_cmdlen(data->cmd) == 1 && cmd->cmd_param[0]
//			&& !builtins(cmd->cmd_param, &data->env))
//			break ;
//		if (cmd->cmd_param[0])
//		{
//			pid = fork();
//			if (pid == -1)
//				return (ft_printf("pid error\n"), 1);
//			if (pid == 0)
//			{
//				close(data->stdin_save);
//				close(data->stdout_save);
//				if (cmd->cmd_param[0] && is_builtins(cmd->cmd_param[0]))
//				{
//					ft_child_builtins(cmd, data->env, prev_fd, fd);
//					free_env(data->env);
//					free_iteration_data(data);
//					close(fd[0]);
//					exit(0);
//				}
//				ft_child(cmd, path_cmd, data->env, prev_fd, fd);
//			}
//			if (prev_fd != -1)
//				close(prev_fd);
//			if (cmd->next)
//			{
//				close(fd[1]);
//				prev_fd = fd[0];
//			}
//			else
//				prev_fd = -1;
//			if (path_cmd && *path_cmd)
//				free(path_cmd);
//			cmd = cmd->next;
//		}
//	}
//	while (head)
//	{
//		waitpid(pid, &error, 0);
//		head = head->next;
//	}
//	return (0);
//}

// int	ft_exec(t_data *data, pid_t pid)
//{
//	t_cmd	*cmd;
//	char	*path_cmd;
//	int		error;
//	int		fd[2];
//	int		prev_fd;
//	char	**lst_path;
//	t_cmd	*head;

//	cmd = data->cmd;
//	head = data->cmd;
//	prev_fd = -1;
//	while (cmd)
//	{
//		path_cmd = NULL;
//		if (cmd->next)
//		{
//			if (pipe(fd) == -1)
//				return (ft_printf("pipe error\n"), 1);
//		}
//		if (cmd->cmd_param[0] && !is_builtins(cmd->cmd_param[0]))
//		{
//			if (!(ft_strchr(cmd->cmd_param[0], '/')
//					&& ft_is_exec(cmd->cmd_param[0], &error)))
//			{
//				lst_path = parse_path(data->env);
//				if (!lst_path || !*lst_path)
//				{
//					ft_free_dtab(lst_path);
//					return (printf("malloc failed parse_path\n"), 1);
//				}
//				path_cmd = search_path(cmd->cmd_param[0], lst_path, &error);
//				ft_free_dtab(lst_path);
//			}
//			else
//				path_cmd = ft_strdup(cmd->cmd_param[0]);
//			if (!path_cmd)
//			{
//				ft_error_msg(cmd->cmd_param[0], "command not found");
//				error = 127;
//				cmd = cmd->next;
//				continue ;
//			}
//			else if (error == 126)
//			{
//				ft_error_msg(cmd->cmd_param[0], "Permission denied");
//				// code error 126
//				if (path_cmd && *path_cmd)
//					free(path_cmd);
//				cmd = cmd->next;
//				continue ;
//			}
//		}
//		if (ft_cmdlen(data->cmd) == 1 && cmd->cmd_param[0]
//			&& !builtins(cmd->cmd_param, &data->env))
//			break ;
//		if (cmd->cmd_param[0])
//		{
//			pid = fork();
//			if (pid == -1)
//				return (ft_printf("pid error\n"), 1);
//			if (pid == 0)
//			{
//				close(data->stdin_save);
//				close(data->stdout_save);
//				if (cmd->cmd_param[0] && is_builtins(cmd->cmd_param[0]))
//				{
//					ft_child_builtins(cmd, data->env, prev_fd, fd);
//					free_env(data->env);
//					free_iteration_data(data);
//					close(fd[0]);
//					exit(0);
//				}
//				ft_child(cmd, path_cmd, data->env, prev_fd, fd);
//			}
//			if (prev_fd != -1)
//				close(prev_fd);
//			if (cmd->next)
//			{
//				close(fd[1]);
//				prev_fd = fd[0];
//			}
//			else
//				prev_fd = -1;
//			if (path_cmd && *path_cmd)
//				free(path_cmd);
//			cmd = cmd->next;
//		}
//	}
//	while (head)
//	{
//		waitpid(pid, &error, 0);
//		head = head->next;
//	}
//	return (0);
//}

// int	main(int ac, char **av, char **env)
//{
//	t_data	data;
//	char	*expended;
//	char	*read;
//	t_token	*tmp_token;
//	int		status;
//	int		stdout_save;
//	int		stdin_save;
//	int		if_infile;
//	pid_t	pid;
//	char	*path_cmd;
//	int		file;
//	int		i;

//	if_infile = 0;
//	status = 0;
//	i = -1;
//		while (env[++i])
//		printf("env==%s\n", env[i]);
//		init_data(&data, ac, av);
//		data.env = cpy_env(env);
//		print_list(data.env);
//		ft_unset(&data.env, ft_split("PATH sd", ' '));
//		stdout_save = dup(STDOUT_FILENO);
//		stdin_save = dup(STDIN_FILENO);
//		while (1)
//	{
//		if (if_infile)
//		{
//			// printf("CACAAAAAAAAAAAAAA\n");
//			close(STDIN_FILENO);
//			dup2(stdin_save, STDIN_FILENO);
//		}
//		read = readline("minishell> ");
//		if (!read)
//			break ;
//		add_history(read);
//		if (!read[0])
//			continue ;
//		expended = expand_env_var(data.env, read);
//		data.token = tokenize(&data, expended);
// cmd_builder(&data);
// free(expended);
// data.cmd = handle_cmd(read);
// print_tokens(data.token);
// free_tokens(data.token);
// print(data.cmd);
// tmp_token = data.token;
// while (tmp_token)
//{
// if (tmp_token->type == REDIR_IN)
//{
//	if (!tmp_token->next)
//	{
//		ft_printf("minishell: syntax error near unexpected token `newline'\n");
//		// code d error 2;
//		break ;
//	}
//	else
//	{
//		tmp_token = tmp_token->next;
//		if (tmp_token->type != WORD)
//		{
//			ft_error_msg(tmp_token->str,
//				"syntax error near unexpected token");
//			//			// coderor2
//						break ;
//					}
//					if (access(tmp_token->str, F_OK) == -1)}
// rl_clear_history();
// free_all(data, read);
// return (0);
//}
//		{
//			ft_error_msg(tmp_token->str,
//				"No such file or directory");
//			// code error 1
//			break ;
//		}
//		else if (access(tmp_token->str, R_OK) == -1)
//		{
//			ft_error_msg(tmp_token->str, "Permission denied");
//			// code error 1
//			break ;
//		}
//		else if (tmp_token->next)
//		{
//			if (redirect_infile(tmp_token->str))
//				return (ft_printf("error redir in\n"), 1);
//			if_infile = 1;
//		}
//		tmp_token = tmp_token->next;
//	}
//}
// else if (tmp_token && tmp_token->type == WORD)
//{
// if (ft_strcmp(tmp_token->str, "env") && ft_strcmp(tmp_token->str,
//		"export") && ft_strcmp(tmp_token->str, "unset")
//	&& ft_strcmp(tmp_token->str, "cd") && ft_strcmp(tmp_token->str,
//		"pwd") && ft_strcmp(tmp_token->str, "echo")
//	&& ft_strcmp(tmp_token->str, "exit"))
//{
//	if (!(ft_strchr(tmp_token->str, '/') && ft_is_exec(tmp_token->str,
//				&status)))
//		path_cmd = search_path(tmp_token->str, parse_path(data.env),
//				&status);
//	else
//		path_cmd = tmp_token->str;
//	if (!path_cmd)
//	{
//		ft_error_msg(tmp_token->str, "command not found");
//		// code error 127
//		break ;
//	}
//	else if (status == 126)
//	{
//		ft_error_msg(tmp_token->str, "Permission denied");
//		// code error 126
//		break ;
//	}
//}
// pid = fork();
// if (pid == -1)
//	return (ft_printf("pid error"), 1);
// else if (pid == 0)
//{
//	builtins(ft_split(tmp_token->str, ' '), &data.env);
//	ft_child(tmp_token, path_cmd, data.env);
//}
// waitpid(pid, &status, 0);
// if (tmp_token->next && (tmp_token->next->type == REDIR_OUT
//		|| tmp_token->next->type == APPEND))
//{
//	if (!tmp_token->next->next)
//	{
//		ft_error_msg("'newline'", "syntax error near unexpected token");
//		// code error 2
//		break ;
//	}
//	// printf("blablabla\n");
//	dup2(STDOUT_FILENO, stdout_save);
//	tmp_token = tmp_token->next->next->next;
//}
// else
//	tmp_token = tmp_token->next;
// if (tmp_token)
//	printf("strcmd=%s\n", tmp_token->str);
// else
//	printf("NULL:)\n");
//}
// else if (tmp_token && tmp_token->type == REDIR_OUT)
//{
//	if (!tmp_token->next)
//		ft_error_msg("'newline'",
//			"syntax error near unexpected token");
//	else
//	{
//		tmp_token = tmp_token->next;
//		file = open(tmp_token->str, O_CREAT | O_WRONLY | O_TRUNC,
//				0644);
//		if (file == -1)
//			return (ft_printf("error open outfile no cmd prev"), 1);
//		close(file);
//		tmp_token = tmp_token->next;
//	}
//}
// else if (tmp_token && tmp_token->type == APPEND)
//{
//	if (!tmp_token->next)
//		ft_error_msg("'newline'",
//			"syntax error near unexpected token");
//	//	else
//	//	{
//	//		tmp_token = tmp_token->next;
//	//		file = open(tmp_token->str, O_CREAT | O_WRONLY | O_APPEND,
//	//				0644);
//	//		if (file == -1)
//	//			return (ft_printf("error open append outfile no cmd prev"),
//	//				1);
//	//		close(file);
//	//		tmp_token = tmp_token->next;
//	//	}
//	//}
//	// else if (tmp_token && tmp_token->type == HEREDOC)
//	//	ft_printf("pas encore de heredoc\n");
//	// if (tmp_token)
//	// tmp_token = tmp_token->next;
//}
//}
// rl_clear_history();
// free_all(data, read);
// return (0);
//}

// int	main(int ac, char **av, char **envp)
//{
//	char	**cmd;
//	char	**path;
//	char	*path_cmd;
//	char	*read;
//	int		status;
//	pid_t	pid;
//	char	*file;
//	t_lst	*env;

//	// char	*tmp_cmd;
//	(void)ac;
//	(void)av;
//	env = parse_env(envp);
//	while (1)
//	{
//		file = NULL;
//		path = parse_path(envp);
//		read = readline("minishell> ");
//		if (!read || (ft_strcmp(read, "exit") == 0))
//		{
//			break ;
//		}
//		if (!read[0])
//			continue ;
//		if (ft_strchr(read, '<'))
//			read = ft_strdup((ft_strchr(read, '<')));
//		cmd = ft_split(read, ' ');
//		printf("read=%s;\n", read);
//		free(read);
//		if (!cmd || !cmd[0])
//			return (ft_error("split error", path, cmd, -1));
//		if (ft_strchr(cmd[0], '<'))
//		{
//			file = cmd[1];
//			cmd += 2;
//		}
//		printf("cmd=%s;\n", cmd[0]);
//		path_cmd = search_path(cmd[0], path, &status);
//		if (!path_cmd)
//		{
//			ft_error("command not found", path, cmd, -1);
//		}
//		else if (status == 126)
//			ft_error("Permission denied", path, cmd, -1);
//		else
//		{
//			pid = fork();
//			if (pid == -1)
//				return (ft_error("pid error", path, cmd, errno));
//			else if (pid == 0)
//			{
//				execve(path_cmd, cmd, envp);
//			}
//			else
//				waitpid(pid, &status, 0);
//		}
//	}
//}

// t_list	*make_env(void)
//{
//		t_list	*env;
//		t_list	*tmp;
//		char	pwd[PATH_MAX];
//		if (!getcwd(pwd, PATH_MAX))
//	{
//		//error getcwd;
//		return (NULL);
//	}
//		tmp = env;
//		env = new_node("OLDPWD");
//		tmp = tmp->next;
//		tmp = new_node() return (env);
//	return (NULL);
//}

// t_list	*parse_env(char **envp) // c du caca
//{
//	t_list *env;
//	t_list *tmp;
//	int i;

//	i = 0;
//	if (!(*envp))
//		return (make_env());
//	env = new_node(envp[i]);
//	if (!env)
//		return (NULL);
//	tmp = env;
//	while (envp[++i])
//	{
//		tmp->next = new_node(envp[i]);
//		tmp = tmp->next;
//		if (!tmp)
//		{
//			ft_free_all_lst(env);
//			return (NULL);
//		}
//	}
//	return (env);
//}
