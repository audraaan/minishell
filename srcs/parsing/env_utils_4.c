/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 05:22:07 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 05:22:37 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quote(int *i, int *quotes, char *str)
{
	if (str[(*i)] == '\'' && (*quotes) != 2)
	{
		if ((*quotes) == 1)
			(*quotes) = 0;
		else
			(*quotes) = 1;
		(*i)++;
		return (1);
	}
	else if (str[(*i)] == '\"' && (*quotes) != 1)
	{
		if ((*quotes) == 2)
			(*quotes) = 0;
		else
			(*quotes) = 2;
		(*i)++;
		return (1);
	}
	return (0);
}

int	exported(t_list **env_cpy, char *arg, t_data *data)
{
	char	**export;
	int		status;

	status = 0;
	if (!arg)
		return (ft_error_msg("make env", "malloc failed"));
	export = malloc(sizeof(char *) * 3);
	if (!export)
		return (ft_error_msg("make env", "malloc failed"));
	export[0] = ft_strdup("export");
	if (!export[0])
	{
		ft_free_dtab(export);
		return (ft_error_msg("make_env: ft_strdup", "malloc failed"));
	}
	export[1] = arg;
	export[2] = NULL;
	status = ft_export(env_cpy, export, data);
	ft_free_dtab(export);
	return (status);
}

int	ft_make_env(t_list **env_cpy, t_data *data)
{
	t_list	*tmp_env;
	char	buf[PATH_MAX];
	char	*cwd;
	char	*pwd;

	pwd = NULL;
	tmp_env = *env_cpy;
	while (tmp_env && tmp_env->name && ft_strcmp(tmp_env->name, "OLDPWD"))
		tmp_env = tmp_env->next;
	if (!tmp_env && exported(env_cpy, ft_strdup("OLDPWD"), data))
		return (1);
	cwd = getcwd(buf, PATH_MAX);
	if (!cwd)
		return ((ft_perror_msg("update_pwd: getcwd", 1)));
	tmp_env = *env_cpy;
	while (tmp_env && tmp_env->name && ft_strcmp(tmp_env->name, "PWD"))
		tmp_env = tmp_env->next;
	if (!tmp_env || !tmp_env->content || ft_strcmp(tmp_env->content, cwd))
	{
		pwd = ft_strjoin("PWD=", cwd);
		if (exported(env_cpy, pwd, data))
			return (1);
	}
	return (update_shlvl(env_cpy, tmp_env, data));
}

int	update_shlvl(t_list **env_cpy, t_list *tmp_env, t_data *data)
{
	char	*shlvl;
	char	*tmp;
	int		level;
	int		res;

	tmp = NULL;
	shlvl = NULL;
	tmp_env = *env_cpy;
	while (tmp_env && tmp_env->name && ft_strcmp(tmp_env->name, "SHLVL"))
		tmp_env = tmp_env->next;
	if (!tmp_env || !tmp_env->content || !ft_str_isdigit(tmp_env->content))
		return (exported(env_cpy, ft_strdup("SHLVL=1"), data));
	level = ft_atoi(tmp_env->content);
	if (level < 0 || (level + 1) < 0)
		return (exported(env_cpy, ft_strdup("SHLVL=0"), data));
	tmp = ft_itoa(level + 1);
	if (!tmp)
		return (ft_error_msg("update_shlvl: ft_itoa", "malloc failed"));
	shlvl = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	if (!shlvl)
		return (ft_error_msg("update_shlvl", "malloc failed"));
	res = exported(env_cpy, shlvl, data);
	return (res);
}
