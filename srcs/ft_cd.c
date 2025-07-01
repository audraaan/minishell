/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:04:32 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/01 10:22:52 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	update_pwd(t_list **env, char *cmd)
{
	char	pwd[PATH_MAX];
	char	**pwds_exported;

	pwds_exported = malloc(sizeof(char *) * 2);
	if (!pwds_exported)
		return (ft_printf("error malloc"));
	if (getcwd(pwd, PATH_MAX))
	{
		pwds_exported[0] = ft_strjoin("OLDPWD=", pwd);
		if (!pwds_exported[0])
			return (ft_printf("malloc failed"));
		pwds_exported[1] = NULL;
		if (ft_export(env, pwds_exported))
			return (ft_printf("error export"));
	}
	else
		return (ft_printf("error getcwd"));
	free(pwds_exported[0]);
	free(pwds_exported[1]);
	pwds_exported[0] = ft_strjoin("PWD=", cmd);
	if (!pwds_exported[0])
		return (ft_printf("malloc failed"));
	pwds_exported[1] = NULL;
	if (ft_export(env, pwds_exported))
		return (ft_printf("error export"));
	ft_free_dtab(pwds_exported);
	return (0);
}

int	ft_cd(t_list **env, char **cmd)
{
	t_list	*tmp;

	tmp = *env;
	if (cmd && !cmd[1])
	{
		while (tmp && ft_strcmp(tmp->name, "HOME"))
			tmp = tmp->next;
		if (!tmp)
			return (ft_error_msg("cd", "HOME not set"));
	}
	else if (cmd && cmd[2])
		return (ft_error_msg("cd", "too many arguments"));
	if (chdir(cmd[1]) == -1)
	{
		perror("cd");
		return (errno);
	}
	return (update_pwd(env, cmd[1]));
}
