/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/16 00:54:25 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	sig_handler(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

t_cmd	*handle_cmd(char *str)
{
	int		len;
	char	**cmd_tab;
	char	*tmp_str;
	char	*new_str;
	t_cmd	*tmp_cmd;
	t_cmd	*cmd;

	if (!str || !(*str))
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	tmp_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->next = tmp_cmd;
	new_str = ft_strdup(str);
	while (new_str)
	{
		if (!tmp_cmd)
			tmp_cmd = (t_cmd *)malloc(sizeof(t_cmd));
		tmp_cmd->next = NULL;
		if (ft_strchr(new_str, '|'))
			len = ft_strlen(new_str) - ft_strlen(ft_strchr(new_str, '|'));
		else
			len = ft_strlen(new_str);
		tmp_str = ft_strndup(new_str, len);
		cmd_tab = ft_split(tmp_str, ' ');
		free(tmp_str);
		tmp_cmd->cmd_param = cmd_tab;
		if (*new_str && ft_strchr(new_str, '|') && &ft_strchr(new_str, '|')[1]
			&& *(&ft_strchr(new_str, '|')[1]))
		{
			tmp_str = new_str;
			new_str = ft_strdup(&ft_strchr(new_str, '|')[1]);
			free(tmp_str);
		}
		else
			new_str = NULL;
		tmp_cmd = tmp_cmd->next;
	}
	return (cmd->next);
}

t_list	*cpy_env(char **env)
{
	int		i;
	int		j;
	char	*name;
	char	*content;
	t_list	*env_cpy;

	env_cpy = NULL;
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		name = ft_substr(env[i], 0, j);
		if (!name)
		{
			free_env(env_cpy);
			return (NULL);
		}
		if (env[i][j] == '=')
			content = ft_strdup(&env[i][j + 1]);
		else
			content = NULL;
		if (env[i][j] == '=' && !content)
		{
			free(name);
			free_env(env_cpy);
			return (NULL);
		}
		ft_lstadd_back(&env_cpy, ft_lstnew(name, content));
		i++;
	}
	return (env_cpy);
}

int	builtins(char **cmd, t_list **env)
{
	if (!ft_strcmp(cmd[0], "env"))
		ft_env(*env);
	else if (!ft_strcmp(cmd[0], "unset"))
		ft_unset(env, cmd);
	else if (!ft_strcmp(cmd[0], "export"))
		ft_export(env, cmd);
	else if (!ft_strcmp(cmd[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd[0], "cd"))
		ft_cd(env, cmd);
	else
		return (1);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*expanded;
	char	*read;
	pid_t	pid;

	init_data(&data, ac, av);
	pid = getpid();
	data.env = cpy_env(env);
	data.stdin_save = dup(STDIN_FILENO);
	data.stdout_save = dup(STDOUT_FILENO);
	if (data.stdin_save == -1 || data.stdout_save == -1)
	{
		ft_printf("error dup save");
		return (1);
	}
	if (!data.env)
	{
		fprintf(stderr, "Error: Failed to copy environment\n");
		return (1);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler);
	while (1)
	{
		dup2(data.stdin_save, STDIN_FILENO);
		dup2(data.stdout_save, STDOUT_FILENO);
		read = readline("minishell> ");
		if (!read || !ft_strcmp(read, "exit"))
		{
			close(data.stdin_save);
			close(data.stdout_save);
			break ;
		}
		if (!read[0])
		{
			free(read);
			continue ;
		}
		add_history(read);
		expanded = expand_env_var(data.env, read);
		free(read);
		if (!expanded)
		{
			fprintf(stderr, "Error: Environment expansion failed\n");
			continue ;
		}
		data.token = tokenize(&data, expanded);
		free(expanded);
		if (!data.token)
			continue ;
		data = cmd_builder(&data);
		// print_tokens(data.token);
		// print(data.cmd);
		ft_exec(&data, pid);
		free_iteration_data(&data);
	}
	rl_clear_history();
	free_all(data, read);
	return (0);
}

int	handle_redir(t_cmd *cmd)
{
	while (cmd->file)
	{
		if (cmd->file->type == REDIR_IN && redirect_infile(cmd->file->file))
			return (ft_printf("error redir in\n"), 1);
		else if (cmd->file->type == REDIR_OUT
			&& redirect_outfile(cmd->file->file))
			return (1);
		else if (cmd->file->type == APPEND
			&& redirect_outfile_append(cmd->file->file))
			return (ft_printf("error redir append\n"), 1);
		cmd->file = cmd->file->next;
	}
	return (0);
}

int	ft_child(t_cmd *cmd, char *path_cmd, t_list *env, int prev_fd, int *fd)
{
	char	**env_exec;

	env_exec = NULL;
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	if (handle_redir(cmd))
		exit(1);
	if (!cmd->cmd_param[0])
		exit(0);
	env_exec = lst_in_tab(env);
	if (!env_exec)
		return (ft_printf("malloc failed\n"), 1);
	execve(path_cmd, cmd->cmd_param, env_exec);
	// free(path_cmd);
	perror("execve");
	exit(errno);
}

int	ft_exec(t_data *data, pid_t pid)
{
	t_cmd	*cmd;
	char	*path_cmd;
	int		error;
	int		fd[2];
	int		prev_fd;
	char	**lst_path;

	cmd = data->cmd;
	prev_fd = -1;
	while (cmd)
	{
		path_cmd = NULL;
		if (cmd->next)
			if (pipe(fd) == -1)
				return (ft_printf("pipe error\n"), 1);
		if (cmd->cmd_param[0] && ft_strcmp(cmd->cmd_param[0], "env")
			&& ft_strcmp(cmd->cmd_param[0], "export")
			&& ft_strcmp(cmd->cmd_param[0], "unset")
			&& ft_strcmp(cmd->cmd_param[0], "cd")
			&& ft_strcmp(cmd->cmd_param[0], "pwd")
			&& ft_strcmp(cmd->cmd_param[0], "echo")
			&& ft_strcmp(cmd->cmd_param[0], "exit"))
		{
			if (!(ft_strchr(cmd->cmd_param[0], '/')
					&& ft_is_exec(cmd->cmd_param[0], &error)))
			{
				lst_path = parse_path(data->env);
				if (!lst_path || !*lst_path)
				{
					ft_free_dtab(lst_path);
					return (printf("malloc failed parse_path\n"), 1);
				}
				path_cmd = search_path(cmd->cmd_param[0], lst_path, &error);
				ft_free_dtab(lst_path);
			}
			else
				path_cmd = ft_strdup(cmd->cmd_param[0]);
			if (!path_cmd)
			{
				ft_error_msg(cmd->cmd_param[0], "command not found");
				error = 127;
				cmd = cmd->next;
				continue ;
			}
			else if (error == 126)
			{
				ft_error_msg(cmd->cmd_param[0], "Permission denied");
				// code error 126
				if (path_cmd && *path_cmd)
					free(path_cmd);
				cmd = cmd->next;
				continue ;
			}
		}
		if (ft_cmdlen(data->cmd) == 1 && cmd->cmd_param[0]
			&& !builtins(cmd->cmd_param, &data->env))
			break ;
		pid = fork();
		if (pid == -1)
			return (ft_printf("pid error\n"), 1);
		// signal(SIGINT, SIG_IGN);
		if (pid == 0)
		{
			close(data->stdin_save);
			close(data->stdout_save);
			if (cmd->cmd_param[0] && !builtins(cmd->cmd_param, &data->env))
			{
				free_env(data->env);
				free_iteration_data(data);
				close(fd[0]);
				exit(0);
			}
			ft_child(cmd, path_cmd, data->env, prev_fd, fd);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		else
			prev_fd = -1;
		if (path_cmd && *path_cmd)
			free(path_cmd);
		cmd = cmd->next;
	}
	while (wait(&error) > 0)
		;
	// waitpid(pid, &error, 0);
	return (0);
}

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
