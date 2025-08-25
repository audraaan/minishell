/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/08/25 14:06:52 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data *data, int ac, char **av)
{
	(void)ac;
	(void)av;
	ft_bzero(data, sizeof(t_data));
	data->stdout_save = -1;
	data->stdin_save = -1;
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->prev_fd = -1;
	if (!data->stdin_save || !data->stdout_save)
	{
		printf("save dup failed\n");
		exit(1);
	}
	g_exit_status = 0;
}

void	ft_dup_std(t_data *data)
{
	if (!isatty(STDIN_FILENO))
		dup2(data->stdin_save, STDIN_FILENO);
	if (!isatty(STDOUT_FILENO))
		dup2(data->stdout_save, STDOUT_FILENO);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*read;
	pid_t	pid;

	if (!isatty(STDOUT_FILENO))
		return (ft_error_msg("STDOUT_FILENO", "not tty"));
	read = NULL;
	init_data(&data, ac, av);
	pid = 0;
	data.env = cpy_env(env, &data);
	rl_event_hook = do_nothing;
	if (!data.env)
		return (ft_error_msg("cpy_env", "Error: Failed to copy environment"));
	data.stdin_save = dup(STDIN_FILENO);
	data.stdout_save = dup(STDOUT_FILENO);
	if (data.stdin_save == -1 || data.stdout_save == -1)
		return (ft_error_msg("dup", "dup failed"));
	set_signals_prompt();
	while (1)
		read = ft_loop(&data, pid, read);
	rl_clear_history();
	free_all(&data, read);
	return (0);
}

char	*ft_loop(t_data *data, pid_t pid, char *read)
{
	ft_dup_std(data);
	read = readline("minishell> ");
	if (g_exit_status)
		data->exit_status = 130;
	g_exit_status = 0;
	if (!read)
		ft_ctrl_d(data, read);
	if (!read[0])
	{
		free(read);
		return (NULL);
	}
	add_history(read);
	data->token = tokenize(data, read);
	free(read);
	if (!data->token || check_synthax(data))
	{
		data->exit_status = 2;
		free_tokens(&(data->token));
		return (NULL);
	}
	update_data(data, pid);
	return (read);
}

void	print(t_cmd *cmd)
{
	int		i;
	int		cmd_num;
	t_file	*file;

	cmd_num = 0;
	if (!cmd)
	{
		printf("Command list is empty\n");
		return ;
	}
	printf("Starting to print commands:\n");
	while (cmd)
	{
		i = 0;
		printf("Command %d at %p:\n", cmd_num++, (void *)cmd);
		if (!cmd->cmd_param)
		{
			printf("  cmd_param array is NULL\n");
		}
		else
		{
			while (cmd->cmd_param[i])
			{
				printf("  param[%d]: '%s'\n", i, cmd->cmd_param[i]);
				i++;
			}
			if (i == 0)
				printf("  No parameters found\n");
		}
		printf("  expended ?: '%d'\n", cmd->expanded);
		file = cmd->file;
		while (file)
		{
			printf("  file: '%s'\n",
				file->filename ? file->filename : "(none)");
			printf("  eof: '%s'\n", file->eof ? file->eof : "(none)");
			printf("  type: '%d'\n", file->type);
			file = file->next;
		}
		cmd = cmd->next;
	}
	printf("Done printing commands\n");
}
void	print_tokens(t_token *head)
{
	const char	*type_names[] = {"REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND",
			"PIPE", "WORD"};

	printf("\n--- TOKENS ---\n");
	while (head)
	{
		printf("[%-8s] %s, %d,%d\n", type_names[head->type], head->str,
			head->q_type, head->in_quote);
		head = head->next;
	}
	printf("--------------\n\n");
}

void	update_data(t_data *data, pid_t pid)
{
	int	is_heredoc;

	//print_tokens(data->token);
	expand_tokens(data);
	//print_tokens(data->token);
	*data = cmd_builder(data);
	//print(data->cmd);
	is_heredoc = handle_heredoc(data);
	signal(SIGINT, SIG_IGN);
	data->exit_status = ft_exec(data, pid);
	if (data->exit_status > 128 && !is_heredoc)
		write(1, "\n", 1);
	set_signals_prompt();
	free_iteration_data(data);
}
