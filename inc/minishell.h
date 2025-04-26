/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:48:52 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/26 10:59:40 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_lst
{
	char			*content;
	struct s_lst	*next;
}					t_lst;

typedef struct s_cmd
{
	char			**cmd_param;
	struct s_cmd	*next;
}					t_cmd;

typedef enum e_token_type
{
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	PIPE,
	WORD
}					t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_data
{
	t_cmd			*cmd;
	t_list			*env;
	t_token			*token;
	char			*fd_in;
	char			*fd_out;
	struct s_data	*next;
}					t_data;

/*parsing*/
void				init_data(t_data *data, int ac, char **av);
t_token				*tokenize(t_data *data, char *str);
t_list				*cpy_env(char **env);
char				*expand_env_var(t_list *env_cpy, char *str);
int					ft_isspace(char c);
void				print_tokens(t_token *head);
void				free_all(t_data data, char *read);

/*utils*/
int					ft_strcmp(char *s1, char *s2);
void				free_tokens(t_token *token);
void				free_env(t_list *env);

// main.c
t_lst				*make_env(void);
t_lst				*parse_env(char **envp);
// build_env.c
int					ft_env(t_lst *env);
int					check_params_env(char *a);
void				ft_unset(t_lst **env, char **a);
// export.c
int					ft_export(t_lst **env, char **a);
// export_not_args.c
int					export_not_args(t_lst **env);

// get_cmd.c
char				**parse_path(char **env);
char				*search_path(char *cmd, char **path, int *error);
// redirect.c
int					redirect_outfile(char *file);
int					redirect_outfile_append(char *file);
int					redirect_infile(char *file);

// utils.c
void				ft_error_msg(char *arg, char *msg);
int					ft_error(char *msg, char **path, char **dtab, int status);
int					ft_free_dtab(char **tab);
int					ft_is_exec(char *path_cmd, int *error);
void				ft_free_all_lst(t_lst *lst);
void				ft_free_lst(t_lst *lst);
t_lst				*new_node(char *str);
int					ft_lstlen(t_lst *lst);
t_lst				*ft_last_node(t_lst *lst);

#endif