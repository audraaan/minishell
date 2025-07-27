/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/27 23:37:01 by alarroye         ###   ########lyon.fr   */
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

void ft_duf_std(t_data *data)
{
	if (!isatty(STDIN_FILENO))
		data->stdin_save = dup2(data->stdin_save, STDIN_FILENO);
	if (!isatty(STDOUT_FILENO))
		dup2(data->stdout_save, STDOUT_FILENO);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*read;
	pid_t	pid;

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
	{
		ft_duf_std(&data);
		read = readline("minishell> ");
		if (g_exit_status)
			data.exit_status = 130;
		g_exit_status = 0;
		if (!read)
		{
			write(data.stdout_save, "exit\n", 5); // if is env
													//-i c coller a l invite
			ft_close_save(&data);
			free_all(&data, read);
			exit(data.exit_status);
			break ;
		}
		if (!read[0])
		{
			free(read);
			continue ;
		}
		add_history(read);
		data.token = tokenize(&data, read);
		free(read);
		if (!data.token || check_synthax(&data))
		{
			data.exit_status = 2;
			free_tokens(&data.token);
			continue ;
		}
		expand_tokens(&data);
		data = cmd_builder(&data);
		// print_list(data.env);
		// print_tokens(data.token);
		// print(data.cmd);
		handle_heredoc(&data);
		signal(SIGINT, SIG_IGN);
		data.exit_status = ft_exec(&data, pid);
		if (data.exit_status > 128)
			write(1, "\n", 1);
		set_signals_prompt();
		free_iteration_data(&data);
	}
	rl_clear_history();
	free_all(&data, read);
	return (0);
}
