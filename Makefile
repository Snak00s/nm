NAME = ft_nm

CC = cc -Wall -Werror -Wextra

SRC = ft_nm.c

OBJ = $(SRC:%.c=%.o)

LIBFT_DIR = libft/

LIBFT = $(LIBFT_DIR)libft.a

all: $(NAME)

%.o: %.c
	$(CC) -c $< -o $@

$(LIBFT) :
	@make -sC $(LIBFT_DIR) all
	@make -sC $(LIBFT_DIR) bonus

$(NAME): $(OBJ) $(LIBFT)
	$(CC) -o $(NAME) $(OBJ) $(LIBFT)

clean:
	@make -sC $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	@make -sC $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re