/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/21 04:17:19 by alarroye         ###   ########lyon.fr   */
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

int	main(int ac, char **av, char **envp)
{
	t_lst	*env;

	(void)ac;
	(void)av;
	env = parse_env(envp);
	ft_unset(&env, av);
	ft_printf("\n");
	export_not_args(&env);
	ft_free_all_lst(env);
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
