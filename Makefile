NAME = ft_nm

CC = cc -Wall -Werror -Wextra -g -Iincludes/

SRCS = srcs/ft_nm.c \
		srcs/utils.c \
		srcs/64b_srcs/convertFormat64.c \
		srcs/64b_srcs/sortSymb64.c \
		srcs/64b_srcs/symb64.c \
		srcs/64b_srcs/symbType64.c \
		srcs/64b_srcs/print64.c \
		srcs/32b_srcs/convertFormat32.c \
		srcs/32b_srcs/sortSymb32.c \
		srcs/32b_srcs/symb32.c \
		srcs/32b_srcs/symbType32.c \
		srcs/32b_srcs/print32.c

OBJ_DIR = obj
OBJ = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = libft/

LIBFT = $(LIBFT_DIR)libft.a

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

$(LIBFT) :
	@make -sC $(LIBFT_DIR) all
	@make -sC $(LIBFT_DIR) bonus

$(NAME): $(OBJ) $(LIBFT)
	$(CC) -o $(NAME) $(OBJ) $(LIBFT)

clean:
	@make -sC $(LIBFT_DIR) clean
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -sC $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re