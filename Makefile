RED=\033[1;31m
GREEN=\033[1;32m
RESET=\033[0m

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

LIBFT_PATH = libs/libft
LIBFT = $(LIBFT_PATH)/libft.a

READLINE_PATH = /usr/include/readline
READLINE_LIB = -lreadline

SRC_FILES = $(shell find ./src -name '*.c')
OBJ_DIR = obj
OBJS = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ -I$(READLINE_PATH) $(READLINE_LIB)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(READLINE_PATH)

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_PATH) fclean
re: fclean all

norm:
	@norminette ./src
	@norminette -R CheckForbiddenSourceHeader ./includes

val:
	@valgrind --track-fds=yes ./minishell

.PHONY: all clean fclean re norm
