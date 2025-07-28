NAME 	= 			libft.a

HEAD 	=			libft.h

OBJ_DIR		=		obj/

CC 	 	=			cc

CFLAGS	=			-Wall -Werror -Wall -MMD -MP

AR		=			ar -rcs

SRC		=			ft_atoi.c \
					ft_bzero.c \
					ft_calloc.c \
					ft_isalnum.c \
					ft_isalpha.c \
					ft_isascii.c \
					ft_isdigit.c \
					ft_isprint.c \
					ft_itoa.c \
					ft_memchr.c \
					ft_memcmp.c \
					ft_memcpy.c \
					ft_memmove.c \
					ft_memset.c \
					ft_putchar_fd.c \
					ft_putendl_fd.c \
					ft_putnbr_fd.c \
					ft_putstr_fd.c \
					ft_split.c \
					ft_strchr.c \
					ft_strdup.c \
					ft_striteri.c \
					ft_strjoin.c \
					ft_strlcat.c \
					ft_strlcpy.c \
					ft_strlen.c \
					ft_strmapi.c \
					ft_strncmp.c \
					ft_strnstr.c \
					ft_strrchr.c \
					ft_strtrim.c \
					ft_substr.c \
					ft_tolower.c \
					ft_toupper.c \
					ft_strcmp.c \
					ft_tools1.c \
					ft_tools2.c \
					ft_printf.c \
					ft_strndup.c \
					get_next_line.c\

SRC_BONUS	=		ft_lstadd_back_bonus.c \
					ft_lstadd_front_bonus.c \
					ft_lstclear_bonus.c \
					ft_lstdelone_bonus.c \
					ft_lstiter_bonus.c \
					ft_lstlast_bonus.c \
					ft_lstmap_bonus.c \
					ft_lstnew_bonus.c \
					ft_lstsize_bonus.c 


OBJ		=			$(SRC:%.c=$(OBJ_DIR)%.o)
OBJ_BONUS	=		$(SRC_BONUS:%.c=$(OBJ_DIR)%.o)
DEP		=			$(SRC:%.c=$(OBJ_DIR)%.d)
DEP_BONUS	=		$(SRC_BONUS:%.c=$(OBJ_DIR)%.d)

# $< = SOURCE FILE && $@ = OUTPUT FILE && $? = DEPS THAT CHANGED

$(OBJ_DIR)%.o: %.c Makefile | $(OBJ_DIR)
						$(CC) $(CFLAGS) -c $< -o $@

all:				$(NAME)

$(NAME):			$(OBJ)
						$(AR) $@ $?

$(OBJ_DIR):
						@mkdir -p $(OBJ_DIR)

bonus: .bonus

.bonus:				$(OBJ) $(OBJ_BONUS)
						$(AR) $(NAME) $?
						@touch .bonus

clean:
						@rm -rf $(OBJ) $(OBJ_DIR)
						@echo "Deleting 'obj'"

fclean:				clean
						@rm -rf $(NAME)
						@echo "Deleting 'libft.a'"

re: 					fclean all

.PHONY: 				clean fclean re all bonus

-include $(DEP) $(DEP_BONUS)
