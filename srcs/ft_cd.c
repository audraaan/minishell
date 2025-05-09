/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:04:32 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/09 15:35:12 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	update_pwd(t_data *data)
{
	char	pwd[PATH_MAX];
	char	**pwds_exported;

	// pwds_exported = NULL;
	if (getcwd(pwd, PATH_MAX))
	{
		pwd = ft_strjoin("PWD=", pwd);
		if (!pwd)
			return (ft_printf("malloc failed"));
		pwds_exported[0] = pwd;
		pwds_exported[1] = ft_strdup("OLDPWD");
		if (!pwds_exported[1])
		{
			ft_free_dtab(pwds_exported);
			return (ft_printf("malloc failed"));
		}
		pwds_exported[2] = NULL;
		return (ft_export(&data->env, pwds_exported));
	}
	else
	{
		perror("getcwd");
		return (errno);
	}
}

int	ft_cd(t_data *data)
{
	t_list	*tmp_env;
	int		res_update;

	tmp_env = data->env;
	if (!data->next)
	{
		while (tmp_env && ft_strcmp(tmp_env->name, "HOME"))
			tmp_env = tmp_env->next;
		if (!tmp_env)
			return (ft_error_msg("cd", "HOME not set"));
	}
	else if (data->next->next->type == WORD)
		return (ft_error_msg("cd", "too many arguments"));
	if (chdir(data->next->cmd->cmd_param[0]) == -1)
	{
		perror("cd");
		return (errno);
	}
	res_update = update_pwd(data);
	if (res_update)
	{
		return (res_update);
	}
}
