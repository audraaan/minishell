/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 22:25:46 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/27 23:45:26 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_done = 1;
	g_exit_status = 130;
}

void	sigint_herdoc_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_done = 1;
	g_exit_status = 130;
	exit(g_exit_status);
}

void	set_signals_prompt(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	do_nothing(void)
{
	return (0);
}
