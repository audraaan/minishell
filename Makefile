# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/16 13:49:07 by alarroye          #+#    #+#              #
#    Updated: 2025/08/06 18:44:19 by alarroye         ###   ########lyon.fr    #
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
								utils_2			\
								free_utils		\
								free_utils_2	\
								free_utils_3	\
								temporary_utils	\
								tokenize 		\
								token_utils		\
								token_utils_2	\
								token_utils_3	\
								env 			\
								env_utils		\
								env_utils_2		\
								env_utils_3		\
								env_utils_4		\
								env_utils_5		\
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
								check_cmd \
								ft_exit \
								ft_echo \
								heredoc \
								utils_exec \
								utils_msg_and_exit \
								signal_utils \
								export_utils \
								retokenize \


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