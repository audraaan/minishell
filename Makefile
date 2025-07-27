# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/16 13:49:07 by alarroye          #+#    #+#              #
#    Updated: 2025/07/27 23:36:33 by alarroye         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #


NAME 				= 			minishell

LIBFT				=			includes/libft/libft.a

LIBS				=			-lreadline

HEAD 				=			minishell.h

OBJ_DIR				=			obj/

CC					=			cc

CFLAGS				=			-Wall -Werror -MMD -MP -g3 -I includes

SRCS_DIR			=			srcs/

VPATH				=			srcs:srcs/parsing	\
VPATH				=			srcs:srcs/exec		\
VPATH				=			srcs:srcs/utils		\
VPATH				=			srcs:srcs/builtins	\

MAIN_SRCS			=			main 			\
								utils 			\
								free_utils		\
								free_utils_2	\
								temporary_utils	\
								tokenize 		\
								token_utils		\
								env 			\
								env_utils		\
								env_utils_2		\
								env_utils_3		\
								env_utils_4		\
								command_builder	\
								builder_utils	\
								builder_utils_2	\
								builtins_env	\
								ft_cd			\
								ft_export		\
								ft_pwd		\
								get_cmd		\
								redirect \
								handle_builtins \
								exec \
								ft_exit \
								ft_echo \
								heredoc \
								utils_exec\
								utils_msg_and_exit\
								signal_utils\
								export_utils

SRCS				=			$(addsuffix .c, $(MAIN_SRCS))

OBJ					=			$(addprefix $(OBJ_DIR),$(SRCS:.c=.o))

DEP					=			$(OBJ:.o=.d)

all:					$(NAME)

$(NAME):				$(OBJ) $(LIBFT)
							$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LIBS) -o $@

$(OBJ_DIR)%.o: %.c
						@mkdir -p $(dir $@)
						$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): FORCE
	$(MAKE) -C includes/libft
	$(MAKE) bonus -C includes/libft

clean:
						make clean -C includes/libft
						@rm -rf $(OBJ_DIR)
						@echo "Deleting $(OBJ_DIR)"


fclean:					clean
							make fclean -C includes/libft
							@rm -rf $(NAME)
							@echo "Deleting $(NAME)"


re: 					fclean
						$(MAKE) all

FORCE:

.PHONY: 				re all clean fclean FORCE

-include $(DEP)