/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:04:32 by alarroye          #+#    #+#             */
/*   Updated: 2025/05/07 15:58:22 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	update_pwd(t_data *data) // pas finit du tt
{
	char **pwd;

	pwd = ft_strdup(data->next->cmd->cmd_param);
	if (!pwd)
		return (ft_printf("malloc failed"));
	pwd = ft_strjoin("PWD=", pwd);
	if (!pwd)
		return (ft_printf("malloc failed"));
	ft_export(&data->env, ODLPWD = PWD);
	ft_export(&data->env, "PWD");
}

int	ft_cd(t_data *data)
{
	t_list	*tmp_env;

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
	if (chdir(data->next->cmd->cmd_param) == -1)
	{
		perror("cd");
		return (errno);
	}
	else
		return (update_pwd(data)); // update de con
}
