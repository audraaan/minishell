/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/07 15:34:27 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_list	*make_env(void)
{
	// t_list	*env;
	// t_list	*tmp;
	// char	pwd[PATH_MAX];
	// if (!getcwd(pwd, PATH_MAX))
	//{
	//	//error getcwd;
	//	return (NULL);
	//}
	// tmp = env;
	// env = new_node("OLDPWD");
	// tmp = tmp->next;
	// tmp = new_node() return (env);
	return (NULL);
}

// t_list	*parse_env(char **envp) // c du caca
//{
//	t_list	*env;
//	t_list	*tmp;
//	int		i;

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

void	print_list(t_list *lst)
{
	while (lst)
	{
		printf("name: %s \ncontent: %s\n\n", lst->name ? lst->name : "(null)",
			lst->content ? lst->content : "(null)");
		lst = lst->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_list	*env;
	char	*jirqf;
	char	**arg;

	(void)ac;
	jirqf = "jiraf bleu= caca";
	arg = ft_split(jirqf, ' ');
	env = cpy_env(envp);
	//ft_export(&env, );
	//ft_export(&env, NULL);
	//ft_printf("\n\n");
	ft_unset(&env, NULL);
	ft_export(&env, NULL);
	// ft_env(env);
	ft_free_all_lst(env);
	ft_free_dtab(arg);
	return (0);
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
int	ft_nb_pipe(t_token *token)
{
	t_token	*node;
	int		res;

	res = 0;
	node = token;
	while (node)
	{
		if (node->type == PIPE)
			res++;
		node = node->next;
	}
	return (res);
}

// void	handle_pipe(int nb_pipe, t_data data)
//{
//	while (nb_pipe)
//	{
//	}
//}

int	is_builtins(char **cmd, t_list **env)
{
	// dprintf(2, "cmd=%s;\n", cmd[0]);
	if (!ft_strcmp(cmd[0], "env") && ft_env(*env) == 0)
	{
		exit(0);
	}
	else if (!ft_strcmp(cmd[0], "unset") && ft_unset(env, cmd) == 0)
		exit(0);
	else if (!ft_strcmp(cmd[0], "export") && ft_export(env, cmd) == 0)
	{
		exit(0);
	}
	else if (!ft_strcmp(cmd[0], "pwd") && ft_pwd() == 0)
	{
		// dprintf(2, "JIRAD");
		exit(0);
	}
	return (0);
}

int	ft_child(t_token *tmp_token, char *path_cmd, char **env)
{
	if (tmp_token->next)
	{
		if (!tmp_token->next->next && (tmp_token->next->type == REDIR_OUT
				|| tmp_token->next->type == APPEND))
			return (ft_printf("minishell: syntax error near unexpected token `newline'\n"));
		if (tmp_token->next->type == REDIR_OUT && tmp_token->next->next
			&& redirect_outfile(tmp_token->next->next->str))
			return (ft_printf("error redir out\n"), 1);
		if (tmp_token->next->type == APPEND && tmp_token->next->next
			&& redirect_outfile_append(tmp_token->next->next->str))
			return (ft_printf("error redir append out\n"), 1);
	}
	return (execve(path_cmd, ft_split(tmp_token->str, ' '), env));
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
//	t_lst	*envp;

//	if_infile = 0;
//	status = 0;
//	init_data(&data, ac, av);
//	envp = parse_env(env);
//	data.env = cpy_env(env);
//	stdout_save = dup(STDOUT_FILENO);
//	stdin_save = dup(STDIN_FILENO);
//	while (1)
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
//		// cmd_builder(&data);
//		free(expended);
//		// data.cmd = handle_cmd(read);
//		print_tokens(data.token);
//		// free_tokens(data.token);
//		// print(data.cmd);
//		tmp_token = data.token;
//		while (tmp_token)
//		{
//			if (tmp_token->type == REDIR_IN)
//			{
//				if (!tmp_token->next)
//				{
//					ft_printf("minishell: syntax error near unexpected token `newline'\n");
//					// code d error 2;
//					break ;
//				}
//				else
//				{
//					tmp_token = tmp_token->next;
//					if (tmp_token->type != WORD)
//					{
//						ft_error_msg(tmp_token->str,
//							"syntax error near unexpected token");
//						// coderor2
//						break ;
//					}
//					if (access(tmp_token->str, F_OK) == -1)
//					{
//						ft_error_msg(tmp_token->str,
//							"No such file or directory");
//						// code error 1
//						break ;
//					}
//					else if (access(tmp_token->str, R_OK) == -1)
//					{
//						ft_error_msg(tmp_token->str, "Permission denied");
//						// code error 1
//						break ;
//					}
//					else if (tmp_token->next)
//					{
//						if (redirect_infile(tmp_token->str))
//							return (ft_printf("error redir in\n"), 1);
//						if_infile = 1;
//					}
//					tmp_token = tmp_token->next;
//				}
//			}
//			else if (tmp_token && tmp_token->type == WORD)
//			{
//				if (ft_strcmp(tmp_token->str, "env")
//					&& ft_strcmp(tmp_token->str, "export")
//					&& ft_strcmp(tmp_token->str, "unset")
//					&& ft_strcmp(tmp_token->str, "cd")
//					&& ft_strcmp(tmp_token->str, "pwd")
//					&& ft_strcmp(tmp_token->str, "echo")
//					&& ft_strcmp(tmp_token->str, "exit"))
//				{
//					if (!(ft_strchr(tmp_token->str, '/')
//							&& ft_is_exec(tmp_token->str, &status)))
//						path_cmd = search_path(tmp_token->str, parse_path(env),
//								&status);
//					else
//						path_cmd = tmp_token->str;
//					if (!path_cmd)
//					{
//						ft_error_msg(tmp_token->str, "command not found");
//						// code error 127
//						break ;
//					}
//					else if (status == 126)
//					{
//						ft_error_msg(tmp_token->str, "Permission denied");
//						// code error 126
//						break ;
//					}
//				}
//				pid = fork();
//				if (pid == -1)
//					return (ft_printf("pid error"), 1);
//				else if (pid == 0)
//				{
//					is_builtins(ft_split(tmp_token->str, ' '), &envp);
//					ft_child(tmp_token, path_cmd, env);
//				}
//				waitpid(pid, &status, 0);
//				if (tmp_token->next && (tmp_token->next->type == REDIR_OUT
//						|| tmp_token->next->type == APPEND))
//				{
//					if (!tmp_token->next->next)
//					{
//						ft_error_msg("'newline'",
//							"syntax error near unexpected token");
//						// code error 2
//						break ;
//					}
//					// printf("blablabla\n");
//					dup2(STDOUT_FILENO, stdout_save);
//					tmp_token = tmp_token->next->next->next;
//				}
//				else
//					tmp_token = tmp_token->next;
//				// if (tmp_token)
//				//	printf("strcmd=%s\n", tmp_token->str);
//				// else
//				//	printf("NULL:)\n");
//			}
//			else if (tmp_token && tmp_token->type == REDIR_OUT)
//			{
//				if (!tmp_token->next)
//					ft_error_msg("'newline'",
//						"syntax error near unexpected token");
//				else
//				{
//					tmp_token = tmp_token->next;
//					file = open(tmp_token->str, O_CREAT | O_WRONLY | O_TRUNC,
//							0644);
//					if (file == -1)
//						return (ft_printf("error open outfile no cmd prev"), 1);
//					close(file);
//					tmp_token = tmp_token->next;
//				}
//			}
//			else if (tmp_token && tmp_token->type == APPEND)
//			{
//				if (!tmp_token->next)
//					ft_error_msg("'newline'",
//						"syntax error near unexpected token");
//				else
//				{
//					tmp_token = tmp_token->next;
//					file = open(tmp_token->str, O_CREAT | O_WRONLY | O_APPEND,
//							0644);
//					if (file == -1)
//						return (ft_printf("error open append outfile no cmd prev"),
//							1);
//					close(file);
//					tmp_token = tmp_token->next;
//				}
//			}
//			else if (tmp_token && tmp_token->type == HEREDOC)
//				ft_printf("pas encore de heredoc\n");
//			// if (tmp_token)
//			// tmp_token = tmp_token->next;
//		}
//	}
//	rl_clear_history();
//	free_all(data, read);
//	return (0);
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
