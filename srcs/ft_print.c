/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:30:09 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/17 05:22:25 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		// ✅ Itérer sans écraser cmd->file
		file = cmd->file;
		while (file)
		{
			printf("  file: '%s'\n", file->file ? file->file : "(none)");
			printf("  type: '%d'\n", file->type);
			file = file->next;
		}
		cmd = cmd->next;
	}
	printf("Done printing commands\n");
}

void	print_list(t_list *lst)
{
	while (lst)
	{
		printf("name: %s \ncontent: %s\n\n", lst->name ? lst->name : "(null)",
			lst->content ? lst->content : "(null)");
		lst = lst->next;
	}
}


void	print_tokens(t_token *head)
{
	const char	*type_names[] = {"REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND",
			"PIPE", "WORD"};

	printf("\n--- TOKENS ---\n");
	while (head)
	{
		printf("[%-8s] %s\n", type_names[head->type], head->str);
		head = head->next;
	}
	printf("--------------\n\n");
}