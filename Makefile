# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/16 13:49:07 by alarroye          #+#    #+#              #
#    Updated: 2025/04/21 03:39:38 by alarroye         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME	= minishell 
CC		=	cc 

CFLAGS	=	-Wall -Wextra -Werror -g3
DFLAGS	=	-MMD -MP

LIBFT_PATH	=	./libft/
LIBFT_FILE	=	libft.a
LIBFT_EX	=	$(LIBFT_PATH)$(LIBFT_FILE)



HDR_FLAG	=	-I$(INC_DIR) -I$(LIBFT_PATH)


SRC_PATH	=	./src/
INC_DIR		=	./inc/

OBJ		=	$(SRC:.c=.o)
DEPS	=	$(OBJ:.o=.d)
SRC		=	$(SRC_PATH)main.c \
			$(SRC_PATH)bultins_env.c \
			$(SRC_PATH)get_cmd.c \
			$(SRC_PATH)redirect.c \
			$(SRC_PATH)utils.c \
			$(SRC_PATH)ft_export_not_args.c \
			$(SRC_PATH)ft_export.c

all: $(NAME)

$(SRC_PATH)%.o: $(SRC_PATH)%.c Makefile
	$(CC) $(CFLAGS) $(DFLAGS) $(HDR_FLAG) -c $< -o $@

-include $(DEPS)


$(NAME): $(LIBFT_EX) $(OBJ)
	$(CC) -lreadline $(CFLAGS) $(OBJ) $(LIBFT_PATH)$(LIBFT_FILE) -o $(NAME)

$(LIBFT_EX): FORCE
	make -C $(LIBFT_PATH)

clean:
	make clean -C $(LIBFT_PATH)
	rm -f $(OBJ)
	rm -f $(DEPS)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_PATH)

re: fclean all

FORCE :

.PHONY: all clean fclean re FORCE