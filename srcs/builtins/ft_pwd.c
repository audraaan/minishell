/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:44:30 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/20 03:54:51 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, PATH_MAX))
	{
		ft_printf("%s\n", pwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (errno);
	}
}
