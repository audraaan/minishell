/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:57:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 10:57:51 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_exit_status(t_data **data, int *i, char *str, char **res)
{
	char	*value;

	value = ft_itoa((*data)->exit_status);
	*res = join_and_free(*res, ft_strdup(value));
	free(value);
	(*i) += 2;
}

void	expand_tokens(t_data *data)
{
	t_token	*current;
	char	*expanded;

	//	char	*quotes_removed;
	current = data->token;
	while (current)
	{
		if (current->type == WORD && current->str)
		{
			expanded = expand_env_var(data, current->str);
			if (!expanded)
			{
				current = current->next;
				continue ;
			}
			//			quotes_removed = remove_quotes(expanded);
			//			free(expanded);
			//			if (!quotes_removed)
			//			{
			//				current = current->next;
			//				continue ;
			//			}
			free(current->str);
			//			current->str = quotes_removed;
			current->str = expanded;
		}
		current = current->next;
	}
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
	return (exported(env_cpy, shlvl, data));
}
