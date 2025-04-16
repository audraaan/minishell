/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:48:52 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/16 13:07:23 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_data
{
	int len_data; // le nb de truc quya ds la ligne de commande
	int type;     // genre si c un file une cmd un < | > >> << t a capter k
	char			**content;
	struct s_data	*next;
}					t_data;

typedef struct s_lst
{
	char			*content;
	struct s_lst	*next;
}					t_lst;

t_lst				*new_node(char *str);
t_lst				*parse_env(char **envp);
int					ft_free_dtab(char **tab);
void				ft_error_msg(char *arg, char *msg);
int					ft_error(char *msg, char **path, char **dtab, int status);
char				**parse_path(char **env);
char				*search_path(char *cmd, char **path, int *error);
int					ft_is_access(char *path_cmd, int *error);

#endif