/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:57:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/21 02:21:43 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include "../../includes/minishell.h"

char	*char_to_str(char c)
{
	char	*str;

	str = malloc(2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = ft_strjoin(s1, s2);
	free(s2);
	free(s1);
	return (str);
}

char	*get_env_value(t_list *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

char	*remove_quotes(const char *src)
{
	char	*res;
	int		i;
	char	q;

	q = 0;
	i = 0;
	res = ft_strdup("");
	while (src[i])
	{
		if ((src[i] == '\'' || src[i] == '"') && q == 0)
			q = src[i++];
		else if (src[i] == q)
		{
			q = 0;
			i++;
		}
		else
			res = join_and_free(res, char_to_str(src[i++]));
	}
	return (res);
}
int	check_unclosed_quotes(int quotes)
{
	if (quotes != 0)
	{
		ft_putstr_fd("minishell: syntax error: quotes unclosed\n", 2);
		return (1);
	}
	return (0);
}
