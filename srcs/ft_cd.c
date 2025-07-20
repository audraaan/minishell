/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:04:32 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/19 23:14:48 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env(t_list **env, char *pwd, char *export)
{
	char	**pwd_exported;
	char	*path_pwd;

	pwd_exported = malloc(sizeof(char *) * 3);
	pwd_exported[2] = NULL;
	pwd_exported[0] = strdup("export");
	if (!pwd_exported[0])
	{
		ft_free_dtab(pwd_exported);
		return (ft_printf("malloc error pwd export"));
	}
	path_pwd = ft_strjoin(export, pwd);
	if (!path_pwd)
	{
		ft_free_dtab(pwd_exported);
		return (ft_printf("malloc failed strjoin path_pwd cd"));
	}
	pwd_exported[1] = path_pwd;
	ft_export(env, pwd_exported);
	ft_free_dtab(pwd_exported);
	return (0);
}

int	fd_cd_not_arg(t_list **env, char *buf)
{
	t_list	*tmp_env;
	char	*pwd;

	tmp_env = *env;
	while (tmp_env && ft_strcmp(tmp_env->name, "HOME"))
		tmp_env = tmp_env->next;
	if (!tmp_env)
		return (ft_error_msg("cd", "HOME not set"));
	pwd = getcwd(buf, PATH_MAX);
	if (!pwd)
	{
		perror("cd: getcwd");
		return (errno);
	}
	if (chdir(tmp_env->content) == -1)
	{
		printf("content:%s;\n", tmp_env->name);
		perror("cd: chdir");
		return (errno);
	}
	update_env(env, tmp_env->content, "PWD=");
	update_env(env, pwd, "OLDPWD=");
	return (0);
}

int	ft_cd(t_list **env, char **cmd)
{
	char	*pwd;
	char	buf[PATH_MAX];

	if (cmd && !cmd[1])
		return (fd_cd_not_arg(env, buf));
	else if (cmd && cmd[2])
		return (ft_error_msg("cd", "too many arguments"));
	pwd = getcwd(buf, PATH_MAX);
	if (!pwd)
	{
		perror("cd: getcwd");
		return (errno);
	}
	if (chdir(cmd[1]) == -1)
	{
		perror("cd");
		return (errno);
	}
	update_env(env, pwd, "OLDPWD=");
	pwd = getcwd(buf, PATH_MAX);
	update_env(env, pwd, "PWD=");
	return (0);
}
