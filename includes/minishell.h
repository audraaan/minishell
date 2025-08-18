/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:48:52 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/06 20:20:02 by alarroye         ###   ########lyon.fr   */
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

typedef enum e_quotes_type
{
	NO_QUOTES,
	SINGLE_QUOTES,
	DOUBLE_QUOTES
}					t_quote_type;

typedef struct s_token
{
	char			*str;
	int				retokenized;
	int				quotes;
	t_token_type	type;
	t_quote_type	q_type;
	struct s_token	*next;
}					t_token;

typedef struct s_data
{
	t_list			*env;
	t_cmd			*cmd;
	t_token			*token;
	t_token			**current_token;
	int				stdout_save;
	int				stdin_save;
	int				fd[2];
	int				prev_fd;
	int				exit_status;
	pid_t			pid;
}					t_data;

extern int			g_exit_status;

/////////////*parsing*/////////////

// tokenize
t_token_type		get_operator_type(char *str, int *i);
t_token				*tokenize(t_data *data, char *str);
t_token				*create_token(char *str, t_token_type type,
						t_quote_type *q_type);
char				*extract_word(char *str, int *i, t_quote_type *q_type);
t_token				*tokenize_bis(int *i, char *str, t_quote_type *q_type);
// token_utils
char				*get_operator_str(t_token_type type);
int					ft_isspace(char c);
int					is_operator(char c);
int					trickster(int *i);
int					needs_space_splitting(char *value);
// token_utils_2
void				handle_double_quote(int *quotes, t_quote_type *q_type);
void				handle_single_quote(int *quotes, t_quote_type *q_type);
void				handle_simple_expansion(char *value, char **res);
void				handle_empty_var(char **res);

// env
char				*expand_env_var(t_data *data, char *str, t_token **current);
void				expand_env_var_bis(t_data *data, char *str, char **res,
						t_token **current);
void				expend_env_var_third(int *i, char *str, t_data *data,
						char **res);
t_list				*cpy_env(char **env, t_data *data);
void				handle_retoken(t_data *data, char *value,
						t_token **current, char **res);
// env_utils
t_list				*create_env_node(char *env_var, t_list **env_cpy);
char				*char_to_str(char c);
char				*join_and_free(char *s1, char *s2);
char				*get_env_value(t_list *env, char *name);
int					check_unclosed_quotes(t_quote_type q_type);
// env_utils_2
void				expand_tokens(t_data *data);
t_token				*process_word_token(t_data *data, t_token *current,
						t_token *next);
int					check_token(t_token **current);
// env_utils_3
void				manage_exit_status(t_data **data, int *i, char *str,
						char **res);
void				replace_current_token_with_list(t_data *data,
						t_token **current, t_token *new_tokens);
int					token_contains_quotes(char *str);
char				*remove_outer_quotes(char *str, t_quote_type q_type);
t_token				*find_prev_token(t_token *head, t_token *token);
// env_utils_4
int					handle_quote(int *i, int *quotes, char *str, t_quote_type q_type);
int					exported(t_list **env_cpy, char *arg, t_data *data);
int					ft_make_env(t_list **env_cpy, t_data *data);
int					update_shlvl(t_list **env_cpy, t_list *tmp_env,
						t_data *data);

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
char				*ft_loop(t_data *data, pid_t pid, char *read);
void				update_data(t_data *data, pid_t pid);
void				sigint_handler(int sig);
void				sigquit_handler(int sig);
void				init_data(t_data *data, int ac, char **av);
int					check_synthax(t_data *data);
t_list				*parse_env(char **envp);
char				*ft_loop(t_data *data, pid_t pid, char *read);

// exec
int					ft_exec(t_data *data, pid_t pid);
void				handle_children(pid_t *pid, t_cmd *cmd, t_data *data,
						char *path_cmd);
int					ft_child(t_cmd *cmd, char *path_cmd, t_data *data);
int					ft_failed_execve(t_data *data, char **cmd, char **env,
						char *path_cmd);
int					ft_wait(t_data *data, pid_t pid);
void				is_cmd_null(t_cmd *cmd, t_data *data);
// check_cmd
void				check_cmd(t_data *data, t_cmd *cmd, char *path);
void				param_is_quotes(char **cmd_param);
void				check_status(t_data *data, char *cmd, char *path);
int					is_sigle_builtins(t_data *data, t_cmd *cmd);

// handle_builtins
int					ft_child_builtins(t_cmd *cmd, t_data *data);
int					is_builtins(t_cmd *cmd);
int					builtins(t_cmd *cmd, t_data *data);

// get_cmd
char				**parse_path(t_list *env);
char				*search_path(char *cmd, char **path, int *error);
char				*ft_path(t_cmd *cmd, t_list *env, int *error);
char				*ft_absolute_path(char *cmd, int *error);
int					ft_status_path(char *cmd, int *error, char *path);

// redirect
int					handle_redir(t_data *data, t_cmd *cmd);
int					ft_loop_redir(t_data *data, t_file *tmp, int status);
int					redirect_outfile(char *file);
int					redirect_outfile_append(char *file);
int					redirect_infile(char *file);
int					redirect_heredoc(char *file);
// heredoc
int					handle_heredoc(t_data *data);
int					ft_heredoc(t_file *tmp, int fd);

/////////////*builtins*/////////////

// env + unset
int					ftexpand_env_var_bis_env(t_list *env, char **cmd);
int					ft_unset(t_list **env, char **a);
int					check_params_env(char *a);
int					ft_env(t_list *env, char **cmd);
// export
int					ft_export(t_list **env, char **a, t_data *data);
void				ft_export_bis(t_list *tmp, t_data *data, char **a, int *i);
int					ft_change_var(t_list **env, char *a, t_data *data);
int					export_not_args(t_list **env);
// export_utils
int					exist(t_list **env, char *a);
char				*expand_value(t_data *data, char *str);
t_list				*create_env_node_from_parts(char *name, char *content);

// ft_pwd
int					ft_pwd(void);
// ft_cd
int					ft_cd(t_list **env, char **cmd, t_data *data);
// ft_exit
int					ft_exit(t_data *data, t_cmd *cmd);
void				ft_ctrl_d(t_data *data, char *read);

// ft_echo
int					ft_echo(char **cmd_param);

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
int					ft_str_isdigit(char *str);
int					ft_free_close_msg(char *msg1, char *msg2, int fd,
						char *read);
void				close_and_free_all(t_data *data);

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
void				*free_return(char *s1, char *s2);
char				**ft_free_and_null(char **tab);

// ft_print
void				print(t_cmd *cmd);
void				print_list(t_list *lst);
void				print_tokens(t_token *head);
void				ft_print_tab(char **tab);

// signaux
void				sigint_handler(int sig);
void				set_signals_prompt(void);
int					do_nothing(void);

// void				expand_env_var_bis(t_data *data, int *quotes, char *str,
//						char **res, t_token **current, int *retokenized);
// extern void			expend_env_var_third(int *i, char *str, t_list *env_cpy,
//						t_data *data, char **res, t_token **current,
//						int *quotes, int *retokenized);
// char				*expand_env_var(t_data *data, char *str, t_token **current);

#endif