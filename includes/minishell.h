/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:48:52 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/23 19:09:11 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all
//  --track-fds=yes --show-mismatched-frees=yes
//-s --trace-children=yes ./minishell

//#define REDIR_IN		0	//>
//#define REDIR_OUT		1	//<
//#define HEREDOC		2	//<<
//#define APPEND		3	//>>
//#define PIPE			4	//|
//#define WORD			5	//cmd

// typedef struct s_list
//{
//	char			*content;
//	char			*name;
//	struct s_list	*next;
//}					t_list;

typedef struct s_lst
{
	char			*content;
	struct s_lst	*next;
}					t_lst;

typedef struct s_file
{
	int				type;
	char			*filename;
	char			*eof;
	struct s_file	*next;
}					t_file;

typedef struct s_cmd
{
	char			**cmd_param;
	t_file			*file;
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
	t_list			*env;
	t_cmd			*cmd;
	t_token			*token;
	int				stdout_save;
	int				stdin_save;
	int				fd[2];
	int				prev_fd;
	int				exit_status;
	pid_t			pid;
}					t_data;

extern int			g_signal_received;

/////////////*parsing*/////////////

// tokenize
t_token_type		get_operator_type(char *str, int *i);
t_token				*tokenize(t_data *data, char *str);
t_token				*create_token(char *str, t_token_type type);
char				*extract_word(char *str, int *i);
t_token				*tokenize_bis(int *i, char *str);
// tokenize_utils
char				*get_operator_str(t_token_type type);
int					ft_isspace(char c);
int					is_operator(char c);
int					trickster(int *i);

// env
t_list				*cpy_env(char **env);
char				*expand_env_var(t_data *data, char *str);
t_list				*create_env_node(char *env_var, t_list **env_cpy);
int					handle_quote(int *i, int *quotes, char *str);
void				expend_env_var_bis(int *i, char *str, t_list *env_cpy,
						char **res);
// env_utils
char				*char_to_str(char c);
char				*join_and_free(char *s1, char *s2);
char				*remove_quotes(const char *src);
char				*get_env_value(t_list *env, char *name);
int					check_unclosed_quotes(int quotes);
// env_utils_2
void				manage_exit_status(t_data **data, int *i, char *str,
						char **res);
void				expand_tokens(t_data *data);

// command_builder
t_data				cmd_builder(t_data *data);
void				cmd_builder_bis(t_token **token, t_cmd **current_cmd,
						int *param_index);
t_cmd				*cmd_list(t_token *token);
int					cmd_list_bis(t_token **current_token, t_cmd **head,
						t_cmd **current);
t_cmd				*create_new_cmd(t_token *token);
// builder_utils
void				cmd_count(t_token *token, int *nb_pipe);
int					get_cmd_size(t_token *token);
int					is_redirection(int type);
t_file				*file_add_back(t_file **lst);
void				copy_filename(t_file *current, t_token **token);
// builder_utils_2
void				handle_redirection(t_file **files, t_token **token);
void				copy_eof(t_file *current, t_token **token);

/////////////*exec*/////////////

// main
void				sigint_handler(int sig);
void				sigquit_handler(int sig);
void				init_data(t_data *data, int ac, char **av);
int					check_synthax(t_data *data);
t_list				*parse_env(char **envp);
void				ft_heredoc(t_file *tmp);

// exec
int					ft_exec(t_data *data, pid_t pid);
pid_t				handle_children(pid_t pid, t_cmd *cmd, t_data *data,
						char *path_cmd);
int					ft_child(t_cmd *cmd, char *path_cmd, t_data *data);
int					ft_failed_execve(t_data *data, char **cmd, char **env,
						char *path_cmd);
int					ft_wait(t_data *data, pid_t pid);

// handle_builtins
int					ft_child_builtins(t_cmd *cmd, t_data *data);
int					is_builtins(t_cmd *cmd);
int					builtins(char **cmd, t_data *data);

// get_cmd
char				**parse_path(t_list *env);
char				*search_path(char *cmd, char **path, int *error);
char				*ft_path(t_cmd *cmd, t_list *env, int *error);

// redirect
int					handle_redir(t_data *data, t_cmd *cmd);
int					redirect_outfile(char *file);
int					redirect_outfile_append(char *file);
int					redirect_infile(char *file);
int					redirect_heredoc(char *file);

/////////////*builtins*/////////////

// builtins_env
int					ft_env(t_list *env);
int					ft_unset(t_list **env, char **a);
int					check_params_env(char *a);

// export
int					ft_export(t_list **env, char **a);
// ft_pwd
int					ft_pwd(void);
// ft_cd
int					ft_cd(t_list **env, char **cmd);

/////////////*other*/////////////
// utils
int					er_msg_free_tok(char *arg, char *msg, t_token **token);
int					ft_perror_msg(char *msg, int error);
int					ft_error_msg(char *arg, char *msg);
int					ft_error(char *msg, char **path, char **dtab, int status);
int					ft_is_exec(char *path_cmd, int *error);
t_list				*new_node(char *str);
int					ft_lstlen(t_list *lst);
t_list				*ft_last_node(t_list *lst);
char				**lst_in_tab(t_list *env);
int					ft_cmdlen(t_cmd *cmd);
void				ft_close_save(t_data *data);
void				ft_free_and_exit(t_data data, char *path_cmd);

// ft_free
void				ft_free_all_lst(t_list *lst);
void				free_file_list(t_file **file);
void				free_tokens(t_token **token);
void				free_env(t_list *env);
int					ft_free_dtab(char **tab);
void				free_all(t_data *data, char *read);
void				free_iteration_data(t_data *data);
void				free_cmd(t_cmd **cmd);
void				ft_free_lst(t_list *lst);

// ft_print
void				print(t_cmd *cmd);
void				print_list(t_list *lst);
void				print_tokens(t_token *head);

#endif