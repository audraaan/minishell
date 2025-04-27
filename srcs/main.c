/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/27 13:22:23 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_lst	*make_env(void)
{
	printf("caca");
	return (NULL);
}

t_lst	*parse_env(char **envp)
{
	t_lst	*env;
	t_lst	*tmp;
	int		i;

	i = 0;
	if (!(*envp))
		return (make_env());
	env = new_node(envp[i]);
	if (!env)
		return (NULL);
	tmp = env;
	while (envp[++i])
	{
		tmp->next = new_node(envp[i]);
		tmp = tmp->next;
		if (!tmp)
		{
			ft_free_all_lst(env);
			return (NULL);
		}
	}
	return (env);
}

// int	main(int ac, char **av, char **envp)
//{
//	t_lst	*env;

//	(void)ac;
//	(void)av;
//	env = parse_env(envp);
//	ft_unset(&env, av);
//	ft_printf("\n");
//	export_not_args(&env);
//	ft_free_all_lst(env);
//	return (0);
//}

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

void	handle_pipe(int nb_pipe, t_data data)
{
	while (nb_pipe)
	{
	}
}
int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*cwd;
	char	*expended;
	char	*read;
	t_token	*tmp_token;
	pid_t	pid;
	int		status;
	char	*path_cmd;

	cwd = NULL;
	status = 0;
	init_data(&data, ac, av);
	data.env = cpy_env(env);
	while (1)
	{
		read = readline("minishell> ");
		if (!read)
			break ;
		add_history(read);
		if (!read[0])
			continue ;
		expended = expand_env_var(data.env, read);
		data.token = tokenize(&data, expended);
		free(expended);
		// data.cmd = handle_cmd(read);
		// print_tokens(data.token);
		// free_tokens(data.token);
		tmp_token = data.token;
		while (tmp_token)
		{
			if (tmp_token->type == 0)
			{
				if (!tmp_token->next)
					ft_printf("minishell: syntax error near unexpected token `newline'\n");
				else
				{
					tmp_token = tmp_token->next;
					if (tmp_token->next)
						if (redirect_infile(tmp_token->str))
							return (ft_printf("error redir in"), 1);
				}
			}
			else if (tmp_token->type == 5)
			{
				path_cmd = search_path(tmp_token->str, parse_path(env),
						&status);
				if (!path_cmd)
				{
					ft_error("command not found", NULL, NULL, -1);
				}
				else if (status == 126)
					ft_error("Permission denied", NULL, NULL, -1);
				pid = fork();
				if (pid == -1)
					return (ft_printf("pid error"), 1);
				else if (pid == 0)
				{
					execve(path_cmd, ft_split(tmp_token->str, ' '), env);
				}
				else
					waitpid(pid, &status, 0);
			}
			else if (tmp_token->type == 1)
			{
				if (!tmp_token->next)
					ft_printf("minishell: syntax error near unexpected token `newline'\n");
				else
				{
					tmp_token = tmp_token->next;
					if (redirect_outfile(tmp_token->str))
						return (ft_printf("error redir out"), 1);
				}
			}
			else if (tmp_token->type == 2)
				ft_printf("pas encore de heredoc\n");
			else if (tmp_token->type == 3)
			{
				tmp_token = tmp_token->next;
				if (redirect_outfile_append(tmp_token->str))
					return (ft_printf("error redir append out"), 1);
			}
			tmp_token = tmp_token->next;
		}
	}
	rl_clear_history();
	free_all(data, read);
	return (0);
}

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
